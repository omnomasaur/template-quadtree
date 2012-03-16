
#include "QuadTree.h"

namespace qt
{
	//Just a simple default for max objects
	template<class Object>unsigned int QuadTree<Object>::sMaxObjects = 30;

	//Default for minimum size
	//template<class Object>Vector2 QuadTree<Object>::sMinSize = Vector2(10, 10);
	template<class Object>Vector2 QuadTree<Object>::sMinSize = Vector2(50, 50);

	template<class Object>QuadTree<Object>::QuadTree(Rectangle Rect, QuadTree* pParent)
		:mRectangle(Rect)
		,mpParent(pParent)
	{
		//If the pointer to our parent is NULL, we are the root node in this quadtree.
		if(pParent == NULL)
		{
			mIsRoot = true;
			mpRoot = this;
		}
		else
		{
			mIsRoot = false;
			//If we are not the root, get the pointer to it from our parent.
			mpRoot = pParent->mpRoot;
		}

		//Add ourself to the list of all nodes
		mpRoot->mNodes.push_back(this);

		//We are obviously a leaf if we have just been created
		mIsLeaf = true;
	}

	template<class Object>QuadTree<Object>::~QuadTree()
	{
		//If we have children, delete them as well
		if(!mIsLeaf)
		{
			delete mpChildren[0][0];
			delete mpChildren[0][1];
			delete mpChildren[1][0];
			delete mpChildren[1][1];
		}

		//Delete the objects we are responsible for
		if(mObjects.size() > 0)
		{
			for(int i = mObjects.size()-1; i >= 0; i--)
			{
				delete mObjects[i].mObject;
			}
		}

		//Sigh, now we have to remove ourself from the vector (and stl vectors suck at having things removed from them)
		if(mpRoot->mNodes.size() > 0)
		{
			for(int i = mpRoot->mNodes.size()-1; i >= 0; i--)
			{
				if(mpRoot->mNodes[i] == this)
				{
					mpRoot->mNodes.erase(mpRoot->mNodes.begin()+i);
					break;
				}
			}
		}
	}

	template<class Object>void QuadTree<Object>::Subdivide(bool divideChildren)
	{
		if(mIsLeaf)
		{
			//Find some values for our child rectangles
			real x = mRectangle.mPosition.x;
			real y = mRectangle.mPosition.y;
			real newWidth = mRectangle.mWidth / 2;
			real newHeight = mRectangle.mHeight / 2;

			//The rectangles for our new children
			Rectangle topLeft = Rectangle(x, y, newWidth, newHeight);
			Rectangle topRight = Rectangle(x + newWidth, y, newWidth, newHeight);
			Rectangle botLeft = Rectangle(x, y + newHeight, newWidth, newHeight);
			Rectangle botRight = Rectangle(x + newWidth, y + newHeight, newWidth, newHeight);

			//Create our children
			mpChildren[0][0] = new QuadTree(topLeft, this);
			mpChildren[1][0] = new QuadTree(topRight, this);
			mpChildren[0][1] = new QuadTree(botLeft, this);
			mpChildren[1][1] = new QuadTree(botRight, this);

			//We are no longer a leaf, so set that
			mIsLeaf = false;

			//Now we need to test if our objects are better placed in one of our children
			//	We test this by putting our objects in a temporary structure, clearing our objects, and then trying to add them to ourself again
			//	If they fit into a child they will fall down into it, if not they will be re-added to our current level
			std::vector<ObjectContainer<Object>> tempObjects = mObjects;
			mObjects.clear();
			if(tempObjects.size() > 0)
			{
				for(int i = tempObjects.size()-1; i >= 0; i--)
				{
					AddObject(tempObjects[i].mObject, tempObjects[i].mRect);
				}
			}
		}
		else if(divideChildren)
		{
			mpChildren[0][0]->Subdivide();
			mpChildren[1][0]->Subdivide();
			mpChildren[0][1]->Subdivide();
			mpChildren[1][1]->Subdivide();
		}
	}

	template<class Object>void QuadTree<Object>::AddObject(Object* object, Rectangle rect)
	{
		//If we're a leaf we can just look at ourself
		if(mIsLeaf)
		{
			//If we have space (or are below or at the minimum size) add the object at this level
			if(mObjects.size()+1 <= sMaxObjects || mRectangle.mWidth <= sMinSize.x || mRectangle.mHeight <=  sMinSize.y)
			{
				//Create our object container
				ObjectContainer<Object> nObjCont;
				nObjCont.mObject = object;
				nObjCont.mRect = rect;

				mObjects.push_back(nObjCont);//Why are you reading this comment? You seriously don't know what push_back() does?

				//Return so we don't fall through.
				return;
			}
			//If not we're going to have to subdivide
			else
			{
				Subdivide();
				//Now we can fall through to the non-leaf version
			}
		}
		//If we're not a leaf we will see if it fits into any of our children.

		//We need a rect for the object, since we're using terrain particles, we just make one up
		Rectangle objectRect = rect;
		//Then we check if fits in our children.  
		if(mpChildren[0][0]->GetRectangle().ContainsRect(objectRect))
		{
			mpChildren[0][0]->AddObject(object, rect);
			return;
		}
		else if(mpChildren[1][0]->GetRectangle().ContainsRect(objectRect))
		{
			mpChildren[1][0]->AddObject(object, rect);
			return;
		}
		else if(mpChildren[0][1]->GetRectangle().ContainsRect(objectRect))
		{
			mpChildren[0][1]->AddObject(object, rect);
			return;
		}
		else if(mpChildren[1][1]->GetRectangle().ContainsRect(objectRect))
		{
			mpChildren[1][1]->AddObject(object, rect);
			return;
		}

		//If it didn't fit into any of our children, we add it here
		
		//Create our object container
		ObjectContainer <Object>nObjCont;
		nObjCont.mObject = object;
		nObjCont.mRect = rect;

		mObjects.push_back(nObjCont);

	}

	template<class Object>void QuadTree<Object>::RemoveObject(Object* object, Rectangle rect)
	{
		//If we're a leaf we can just look at ourself
		if(mIsLeaf)
		{
			for(int i = 0; i < mObjects.size(); i++)
			{
				if(mObjects[i].mObject == object)
				{
					mObjects.erase(mObjects.begin()+i);
					
					//We don't want to fall through since we already completed the operation
					return;
				}
			}
		}
		//If we're not a leaf we will see if it is in any of our children.

		Rectangle objectRect = rect;
		//Then we check if fits in our children.  
		if(mpChildren[0][0]->GetRectangle().ContainsRect(objectRect))
		{
			mpChildren[0][0]->RemoveObject(object, rect);
			return;
		}
		else if(mpChildren[1][0]->GetRectangle().ContainsRect(objectRect))
		{
			mpChildren[1][0]->RemoveObject(object, rect);
			return;
		}
		else if(mpChildren[0][1]->GetRectangle().ContainsRect(objectRect))
		{
			mpChildren[0][1]->RemoveObject(object, rect);
			return;
		}
		else if(mpChildren[1][1]->GetRectangle().ContainsRect(objectRect))
		{
			mpChildren[1][1]->RemoveObject(object, rect);
			return;
		}

		//If it didn't fit into any of our children, it's probably here
		for(int i = 0; i < mObjects.size(); i++)
		{
			if(mObjects[i].mObject == object)
			{
				mObjects.erase(mObjects.begin()+i);
				break;
			}
		}

	}

	template<class Object>void QuadTree<Object>::DeleteObject(Object* object, Rectangle rect)
	{
		//If we're a leaf we can just look at ourself
		if(mIsLeaf)
		{
			for(int i = 0; i < mObjects.size(); i++)
			{
				if(mObjects[i].mObject == object)
				{
					delete mObjects[i].mObject;
					mObjects.erase(mObjects.begin()+i);
					
					//We don't want to fall through since we already completed the operation
					return;
				}
			}
		}
		//If we're not a leaf we will see if it is in any of our children.

		Rectangle objectRect = rect;
		//Then we check if fits in our children.  
		if(mpChildren[0][0]->GetRectangle().ContainsRect(objectRect))
		{
			mpChildren[0][0]->DeleteObject(object, rect);
			return;
		}
		else if(mpChildren[1][0]->GetRectangle().ContainsRect(objectRect))
		{
			mpChildren[1][0]->DeleteObject(object, rect);
			return;
		}
		else if(mpChildren[0][1]->GetRectangle().ContainsRect(objectRect))
		{
			mpChildren[0][1]->DeleteObject(object, rect);
			return;
		}
		else if(mpChildren[1][1]->GetRectangle().ContainsRect(objectRect))
		{
			mpChildren[1][1]->DeleteObject(object, rect);
			return;
		}

		//If it didn't fit into any of our children, it's probably here
		for(int i = 0; i < mObjects.size(); i++)
		{
			if(mObjects[i].mObject == object)
			{
				delete mObjects[i].mObject;
				mObjects.erase(mObjects.begin()+i);
				break;
			}
		}

	}

	template<class Object>void QuadTree<Object>::Clear()
	{
		//If we have children, delete them as well
		if(mIsRoot && !mIsLeaf)
		{
			delete mpChildren[0][0];
			delete mpChildren[0][1];
			delete mpChildren[1][0];
			delete mpChildren[1][1];

			mIsLeaf = true;
		}

		//Delete the objects we are responsible for
		if(mObjects.size() > 0)
		{
			for(int i = mObjects.size()-1; i >= 0; i--)
			{
				delete mObjects[i].mObject;
			}
		}
		mObjects.clear();
	}

	template<class Object>void QuadTree<Object>::GetObjects(std::vector<Object*>* rVector, Rectangle Rect)
	{
		Vector2 collDepth = Rectangle::CollideDepth(mRectangle, Rect);
		if(collDepth != Vector2(0, 0))
		{
			for(int i = 0; i < mObjects.size(); i++)
			{
				rVector->push_back(mObjects[i].mObject);
			}

			if(!mIsLeaf)
			{
				mpChildren[0][0]->GetObjects(rVector, Rect);
				mpChildren[1][0]->GetObjects(rVector, Rect);
				mpChildren[0][1]->GetObjects(rVector, Rect);
				mpChildren[1][1]->GetObjects(rVector, Rect);
			}
		}
	}

}//namespace qt