//File: QuadTree.h
//Author: Eric Kasper
//About: This is a quadtree using templates and stl.
//More info: Wikipedia for quadtrees.

#pragma once

#include "Rectangle.h"
#include <vector>

namespace qt
{
	template<class Object>

	struct ObjectContainer
	{
		Object* mObject;
		Rectangle mRect;
	};
	
	template<class Object>

	class QuadTree
	{
	public: 
		//The max number of objects per node
		static unsigned int sMaxObjects;
		//The minimum size of a quadtree
		static Vector2 sMinSize;

		QuadTree(Rectangle Rect, QuadTree* pParent = NULL);
		~QuadTree();

		void Subdivide(bool divideChildren = false);

		//Adds given object to the quadtree
		void AddObject(Object* object, Rectangle rect);

		//Removes an object from the quadtree, but does not delete it
		void RemoveObject(Object* object, Rectangle rect);

		//Deletes an object from the quadtree
		void DeleteObject(Object* object, Rectangle rect);

		//Deletes all objects and gets rid of all but the root node
		void Clear();

		void GetObjects(std::vector<Object*>* rVector, Rectangle Rect);
		//ObjectArray* GetObjects(Rectangle Rect);

		//Getters / Setters / Etc.
		Rectangle GetRectangle() const { return mRectangle; }

		QuadTree* GetParent() const { return mpParent; }

		bool IsLeaf() const { return mIsLeaf; }

		bool IsRoot() const { return mIsRoot; }

		//Members
		std::vector<ObjectContainer<Object>> mObjects;//Responsible

	private:
		Rectangle mRectangle;

		//At the moment there is no reason to actually look at the parent, 
		//other than to discern whether or not we are in the root node.
		QuadTree* mpParent;//Not responsible
		bool mIsLeaf;
		QuadTree* mpChildren[2][2];//Responsible

		//Things that the root needs to keep track of
		bool mIsRoot;
		//Some need to be public
		public:
		QuadTree* mpRoot;//Not responsible
		std::vector<QuadTree*> mNodes;
	};
}