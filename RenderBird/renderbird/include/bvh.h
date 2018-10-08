#pragma once
#include "RenderbirdPrivate.h"
#include "AccelStructure.h"

namespace RenderBird
{
	class BVH : public AccelStructure
	{
		static const uint8 C_Bvh_Num_Plane_Set = 7;
		static const Vector3f planeSetNormals[C_Bvh_Num_Plane_Set];
		static const uint32 C_Bvh_Max_Depth = 16;
		struct Extents
		{
			Extents()
			{
				for (uint8 i = 0; i < C_Bvh_Num_Plane_Set; ++i)
					d[i][0] = C_FLOAT_MAX, d[i][1] = -C_FLOAT_MAX;
			}
			void ExtendBy(const Extents& e)
			{
				for (uint8 i = 0; i < C_Bvh_Num_Plane_Set; ++i) {
					if (e.d[i][0] < d[i][0]) d[i][0] = e.d[i][0];
					if (e.d[i][1] > d[i][1]) d[i][1] = e.d[i][1];
				}
			}
			/* inline */
			Vector3f Center() const
			{
				return Vector3f(
					d[0][0] + d[0][1] * 0.5,
					d[1][0] + d[1][1] * 0.5,
					d[2][0] + d[2][1] * 0.5);
			}
			bool Intersect(const Float*, const Float*, Float&, Float&, uint8&) const;
			Float d[C_Bvh_Num_Plane_Set][2];
			//const Mesh* mesh;
			const SceneObject* m_obj;
		};

		struct Octree
		{
			Octree(const Extents& sceneExtents)
			{
				Float xDiff = sceneExtents.d[0][1] - sceneExtents.d[0][0];
				Float yDiff = sceneExtents.d[1][1] - sceneExtents.d[1][0];
				Float zDiff = sceneExtents.d[2][1] - sceneExtents.d[2][0];
				Float maxDiff = std::max(xDiff, std::max(yDiff, zDiff));
				Vector3f vecDiff(maxDiff, maxDiff, maxDiff);
				Vector3f minPlusMax(
					sceneExtents.d[0][0] + sceneExtents.d[0][1],
					sceneExtents.d[1][0] + sceneExtents.d[1][1],
					sceneExtents.d[2][0] + sceneExtents.d[2][1]);
				bbox.m_min = (minPlusMax - vecDiff) * 0.5;
				bbox.m_max = (minPlusMax + vecDiff) * 0.5;
				root = new OctreeNode;
			}

			~Octree() { DeleteOctreeNode(root); }

			void Insert(const Extents* extents) { Insert(root, extents, bbox, 0); }
			void Build() { Build(root, bbox); };

			struct OctreeNode
			{
				OctreeNode* child[8] = { nullptr };
				std::vector<const Extents *> nodeExtentsList; // pointer to the objects extents 
				Extents nodeExtents; // extents of the octree node itself 
				bool isLeaf = true;
			};

			struct QueueElement
			{
				const OctreeNode *node; // octree node held by this element in the queue 
				Float t; // distance from the ray origin to the extents of the node 
				QueueElement(const OctreeNode *n, Float tn) : node(n), t(tn) {}
				// priority_queue behaves like a min-heap
				friend bool operator < (const QueueElement &a, const QueueElement &b) { return a.t > b.t; }
			};

			OctreeNode* root = nullptr; // make unique son don't have to manage deallocation 
			BoundingBox bbox;
		private:
			void DeleteOctreeNode(OctreeNode*& node)
			{
				for (uint8 i = 0; i < 8; i++) 
				{
					if (node->child[i] != nullptr) 
					{
						DeleteOctreeNode(node->child[i]);
					}
				}
				delete node;
			}

			void Insert(OctreeNode*& node, const Extents* extents, const BoundingBox& bbox, uint32 depth)
			{
				if (node->isLeaf) 
				{
					if (node->nodeExtentsList.size() == 0 || depth == C_Bvh_Max_Depth)
					{
						node->nodeExtentsList.push_back(extents);
					}
					else 
					{
						node->isLeaf = false;
						// Re-insert extents held by this node
						while (node->nodeExtentsList.size()) 
						{
							Insert(node, node->nodeExtentsList.back(), bbox, depth);
							node->nodeExtentsList.pop_back();
						}
						// Insert new extent
						Insert(node, extents, bbox, depth);
					}
				}
				else 
				{
					// Need to compute in which child of the current node this extents should
					// be inserted into
					Vector3f extentsCentroid = extents->Center();
					Vector3f nodeCentroid = (bbox.m_min + bbox.m_max) * 0.5;
					BoundingBox childBBox;
					uint8 childIndex = 0;
					// x-axis
					if (extentsCentroid.x > nodeCentroid.x) 
					{
						childIndex = 4;
						childBBox.m_min.x = nodeCentroid.x;
						childBBox.m_max.x = bbox.m_max.x;
					}
					else 
					{
						childBBox.m_min.x = bbox.m_min.x;
						childBBox.m_max.x = nodeCentroid.x;
					}
					// y-axis
					if (extentsCentroid.y > nodeCentroid.y) 
					{
						childIndex += 2;
						childBBox.m_min.y = nodeCentroid.y;
						childBBox.m_max.y = bbox.m_max.y;
					}
					else 
					{
						childBBox.m_min.y = bbox.m_min.y;
						childBBox.m_max.y = nodeCentroid.y;
					}
					// z-axis
					if (extentsCentroid.z > nodeCentroid.z) 
					{
						childIndex += 1;
						childBBox.m_min.z = nodeCentroid.z;
						childBBox.m_max.z = bbox.m_max.z;
					}
					else 
					{
						childBBox.m_min.z = bbox.m_min.z;
						childBBox.m_max.z = nodeCentroid.z;
					}

					// Create the child node if it doesn't exsit yet and then insert the extents in it
					if (node->child[childIndex] == nullptr)
						node->child[childIndex] = new OctreeNode;
					Insert(node->child[childIndex], extents, childBBox, depth + 1);
				}
			}

			void Build(OctreeNode*& node, const BoundingBox& bbox)
			{
				if (node->isLeaf) 
				{
					for (const auto& e : node->nodeExtentsList) 
					{
						node->nodeExtents.ExtendBy(*e);
					}
				}
				else 
				{
					for (uint8 i = 0; i < 8; ++i) 
					{
						if (node->child[i]) 
						{
							BoundingBox childBBox;
							Vector3f centroid = bbox.GetCenter();
							// x-axis
							childBBox.m_min.x = (i & 4) ? centroid.x : bbox.m_min.x;
							childBBox.m_max.x = (i & 4) ? bbox.m_max.x : centroid.x;
							// y-axis
							childBBox.m_min.y = (i & 2) ? centroid.y : bbox.m_min.y;
							childBBox.m_max.y = (i & 2) ? bbox.m_max.y : centroid.y;
							// z-axis
							childBBox.m_min.z = (i & 1) ? centroid.z : bbox.m_min.z;
							childBBox.m_max.z = (i & 1) ? bbox.m_max.z : centroid.z;

							// Inspect child
							Build(node->child[i], childBBox);

							// Expand extents with extents of child
							node->nodeExtents.ExtendBy(node->child[i]->nodeExtents);
						}
					}
				}
			}
		};

		std::vector<Extents> extentsList;
		Octree* octree = nullptr;
	public:
		BVH(const std::vector<SceneObject*>& sceneObjects);
		virtual bool Intersect(const Ray& ray, RayHitInfo* hitInfo)const;
		~BVH() { delete octree; }
	};
}