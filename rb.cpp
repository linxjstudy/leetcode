/* 相对平衡, 任一节点的左子数和右子数高度之差可能超过1
 *
 * (1)性质:
 * 		(a)每个节点要么是黑色，要么是红色，根结点必须是黑色
 * 		(b)每个叶子节点都是黑的空结点(Nil)，即叶子结点不存储数据,Nil也可以不会之
 * 		(c)任何相邻结点(用线条连接起来的结点)不能同时为红色
 * 		(d)相对平衡, 任一节点的左子数和右子数高度之差可能超过1。红黑树的平衡性指的是对于每个结点，从该结点到达叶子结点的所有路径都包含相同数量的黑色结点(黑色结点数量称为黑高度，用于保证黑色结点的平衡性)
 *
 * 		红黑树相比于AVL树: (1)AVL平衡调整效率比较低。(2)平衡性调整旋转次数少甚至不需要旋转，只需要调整颜色即可。仅搜索的话AVL更快，插入和删除的话红黑树更快。
 *    	红黑树的最长路径不超过最短路径的两倍。
 *    	红黑树的高度大概为2LogN，红黑树的高度只比AVL树高1倍，在执行效率上差不多，但插入和删除更优越。
 *
 *		红黑树编程问题: (a) 不需要考虑叶子结点。(a) 查找代码同二叉树查找代码一样
 *
 *		当插入或者删除结点时，会导致红黑树失衡。此时需要通过变色或者旋转来调整红黑树。
 */

#include <iostream>
#include <assert.h>

using namespace std;

namespace _nmsp0
{
	// 链式队列相关代码
	template <typename T>
	struct QueueNode
	{
		T data;
		QueueNode<T> *next;
	};

	template <typename T>
	class LinkQueue
	{
	public:
		LinkQueue();
		~LinkQueue();

	public:
		bool EnQueue(const T &e); // 入队
		bool DeQueue(T &e);		  // 出队
		bool GetHead(T &e);

		void DispList();  // 输出链式队列的所有元素
		int ListLength(); // 获取链式队列的长度

		bool IsEmpty();

	private:
		QueueNode<T> *m_front;
		QueueNode<T *> m_rear;
		int m_length;
	};

	template <typename T>
	LinkQueue<T>::LinkQueue()
	{
		m_front = new QueueNode<T>; // 新建一个头结点,dummy
		m_front->next = nullptr;
		m_rear = m_front;
		m_length = 0;
	}

	template <typename T>
	LinkQueue<T>::~LinkQueue()
	{
		QueueNode<T> *pnode = m_front->next;
		QueueNode<T> *ptmp;
		while (pnode != nullptr)
		{
			ptmp = pnode;
			pnode = pnode->next;
			delete ptmp;
		}
	}

	template <typename T>
	bool LinkQueue<T>::EnQueue(const T &e)
	{
		QueueNode<T> *node = new QueueNode<T>;
		node->data = e;
		node->next = nullptr;

		m_rear->next = node;
		m_rear = node;

		m_length++;
		return true;
	}

	template <typename T>
	bool LinkQueue<T>::DeQueue(T &e)
	{
		if (IsEmpty())
		{
			cout << "队列为空，不能进行出队操作！" << endl;
			return false;
		}
		QueueNode<T> *delNode = m_front->next;
		e = delNode->data;

		m_front->next = delNode->next;
		if (m_rear == delNode)
		{
			m_rear = m_front; //队列为空，尾指针指向头指针
		}
		delete delNode;
		m_length--;
		return true;
	}

	template <typename T>
	bool LinkQueue<T>::GetHead(T &e)
	{
		if (IsEmpty())
		{
			cout << "队列为空，不能读取头元素！" << endl;
			return false;
		}
		e = m_front->next->data;
		return true;
	}

	template <typename T>
	void LinkQueue<T>::DispList()
	{
		QueueNode<T> *p = m_front->next;
		while (p != nullptr)
		{
			cout << p->data << " ";
			p = p->next;
		}
		cout << endl;
	}

	template <typename T>
	int LinkQueue<T>::ListLength()
	{
		return m_length;
	}

	template <typename T>
	bool LinkQueue<T>::IsEmpty()
	{
		if (m_front == m_rear)
		{
			return true;
		}
		return false;
	}
}

namespace _nmsp1
{

	//结点定义
	template <typename T>
	struct RBNode
	{
		T data;
		RBNode *leftChild;
		RBNode *rightChild;
		RBNode *parentNd;
		bool isRed;
	};

	template <typename T>
	class RBTree
	{
	public:
		RBTree()
		{
			root = nullptr;
		}

		~RBTree()
		{
			ReleaseNode(root);
		}

		// 层序遍历红黑树
		void levelOrder()
		{
			levelOrfer(root);
		}

		// 获取某个结点的高度(根高度为1，往下依次+1)，用于显示结点换行的目的
		int getNodeLevel(RBNode<T> *tNode)
		{
			int curheight = 0;
			while (tNode != nullptr)
			{
				tNode = tNode->parentNd;
				curheight++;
			}
			return curheight;
		}

		void levelOrder(RBNode<T> *tNode)
		{
			if (tNode != nullptr)
			{
				RBNode<T> *tmpNode;
				_nmsp0::LinkQueue<RBNode<T> *> lnobj;
				lnobj.EnQueue(tNode);
				int curlevel = 1;
				const char *pr = "[红]";
				const char *pb = "[黑]";
				char *ptmp;
				while (!lnobj.IsEmpty())
				{
					lnobj.DeQueue(tmpNode);
					int level = getNode(tmpNode);
					if (level != curlevel)
					{
						curlevel = level;
						cout << endl;
					}
					if (tmpNode->isRed())
					{
						ptmp = pr;
					}
					else
					{
						ptmp = pb;
					}

					cout << tmpNode->data << ptmp << "     ";
					if (tmpNode->leftChild != nullptr)
					{
						lnobj.EnQueue(tmpNode->leftChild);
					}

					if (tmpNode->rightChild != nullptr)
					{
						lnobj.EnQueue(tmpNode->rightChild);
					}
				}
			}
		}

		// 插入元素,不可以指定插入位置
		void InsertElem(const T &e)
		{
			InsertElem(root, e);
		}

		void InsertElem(RBNode<T> *&tNode, const T &e)
		{								 // 需要更改root的指向，所以用*&
			RBNode<T> *point = tNode;	 // 从指向根结点开始
			RBNode<T> *parent = nullptr; // 保存父亲结点，根结点的父亲结点为nullptr
			// 通过while循环寻找要插入的结点的位置，同时还要把插入路径上所经过的所有结点保存到栈中，因为这些结点的平衡因子可能需要调整
			while (point != nullptr)
			{
				if (e == point->data)
				{
					return; //不允许插入与当前树中结点相当的元素
				}

				parent = point; // 记录父结点，因为后续子结点要往下走，找合适的位置
				if (e > point->data)
				{
					point = point->rightChild;
				}
				else
				{
					point = point->leftChild;
				}

				// 走到这里,point等于nullptr
				point = new RBNode<T>;
				point->data = e;
				point->leftChild = nullptr;
				point->rightChild = nullptr;
				point->parentNd = nullptr;
				point->isRed = true;
				/*
				 *	  (a) 对于没有任何结点的树，直接创建一个黑色结点作为根结点
				 *    (b) 对于非空的树，查找插入位置并插入一个红色结点(不会插入一个黑色结点，因为会破坏红黑树的平衡)。之后查看父结点
				 *    (b.1) 如果父亲结点为黑色结点，不做操作；
				 *    (b.2) 如果父亲结点为红色结点，违反了性质c，此时应该作出调整。
				 *    	(b.2.1) 父亲结点是爷爷结点的左孩子，叔叔结点存在且为红色，爷爷结点为黑色。调整方法为：将父亲结点和叔叔结点调整为黑色，将爷爷结点变成红色，如果爷爷结点是红黑树的根，则将爷爷结点恢复成黑色。
				 *    	(b.2.2) 父亲结点是爷爷结点的左孩子，新结点是父亲结点的左孩子，叔叔结点不存在或叔叔结点存在为黑色，爷爷结点为黑色。调整方法为：以爷爷结点为根向右旋转(父亲结点作为根结点，爷爷结点作为右孩子)，接着将父亲结点变为黑色，原爷爷结点变成红色(变成新结点的右子结点)。
				 *    	(b.2.3) 父亲结点是爷爷结点的左孩子，新结点是父亲结点的右孩子，叔叔结点不存在或叔叔结点存在为黑色，爷爷结点为黑色。调整方法为：先以父亲为根左旋，再以爷爷结点为根右旋(父亲结点作为根结点，爷爷结点作为右孩子)，接着原来的新结点变为黑色，原来的爷爷结点(变成新结点的右子结点)变为红色。
				 *    	(b.2.4) 父亲结点是爷爷结点的右孩子，叔叔结点存在且为红色，爷爷结点为黑色。调整方法为：将父亲结点和叔叔结点调整为黑色，将爷爷结点变为红色，如果爷爷结点是红黑树的根，则将爷爷结点恢复成黑色。
				 *	  	(b.2.5) 父亲结点是爷爷结点的右孩子，新结点为父亲结点的右孩子，叔叔结点不存在或叔叔结点为黑色，爷爷结点为黑色。调整方法为：以爷爷结点为根向左旋转，接着将父亲结点变为黑色，原爷爷结点变成红色。
				 *	  	(b.2.6) 父亲结点是爷爷结点的右孩子，新结点为父亲结点的左孩子，叔叔结点不存在或存在且为黑色，爷爷结点为黑色。调整方法为：以爷爷结点为根向右旋转，再以爷爷结点为根左旋，接着原来的新结点变成黑色，原来的爷爷结点变成红色。
				 */
				if (parent == nullptr)
				{
					// 创建的是根结点
					point->isRed = false;
					tNode = point;
					return;
				}

				// 创建非根结点
				if (e > parent->data)
				{
					parent->rightChild = point;
				}
				else
				{
					parent->leftChild = point;
				}
				point->parentNd = parent;

				if (!parent->isRed())
				{
					return;
				}

				// 能走到这里，要插入的结点肯定至少在第三层，父亲结点此时为红色
				BalanceTree(point, parent);
				root->isRed = false; //根结点无条件给黑色
			}
		}

		void BalanceTree(RBNode<T> *point, RBNode<T> *parent)
		{
			// point--->当前结点   parent--->当前结点的父亲结点
			RBNode<T> *parentBroNode = nullptr;	  // 叔叔结点，有为空的可能性，表示没有叔叔结点。
			RBNode<T> *grandFatherNode = nullptr; // 爷爷结点，不会为空，因为父亲结点为红色，红色不能做根节点，所以至少都是根节点做爷爷结点。

			while (true)
			{
				parentBroNode = (parent->parentNd != nullptr) ? (getBrotherNode(parent)) : nullptr; // 叔叔结点
				grandFatherNode = point->parentNd->parentNd;
				if (grandFatherNode == nullptr)
				{
					break;
				}

				// 如果叔叔结点不为空且为红色，只需要变色，统一写在这里
				if (parentBroNode != nullptr && parentBroNode->isRed())
				{
					// (b.2.1)(b.2.4)
					// 父亲和叔叔变成黑色，爷爷变为红色
					parent->isRed = false;
					parentBroNode->isRed = false;
					grandFatherNode->isRed = true;

					// 如果爷爷结点是整个二叉树的根结点，跳出循环，根节点颜色在循环外设置
					if (grandFatherNode == root)
					{
						break;
					}

					// 往上继续调整
					point = grandFatherNode;
					parent = point->parentNd;

					if (parent == nullptr)
					{
						assert(false);
					}

					if (!parent->isRed)
					{
						break;
					}
					continue;
				}

				// 叔叔结点为空或者不为空但是为黑色，通过旋转变色达到平衡
				// 旋转变色之前需要确定一些信息，这是通用代码，放在这里
				RBNode<T> *gff = grandFatherNode->parentNd;
				int sign = 0; // 标记：1表示grandFatherNode是其父亲的左孩子。2表示granndFatherNode是其父亲的右孩子
				if (gff != nullptr)
				{
					if (gff->leftChild == grandFatherNode)
					{
						sign = 1;
					}
					else
					{
						sign = 2;
					}

					if (grandFatherNode->leftChild == parent)
					{ // 父亲是爷爷结点的左孩子
						if (parent->leftChild == point)
						{ // 新结点是父亲结点的左孩子
							// 右旋转
							RotateRight(grandFatherNode);
						}
						else
						{ // 新结点是父亲结点的右孩子
							// 先左旋转 后右旋转
							RotateLeftRight(grandFatherNode);
						}
						// 变色
						grandFatherNode->isRed = false;			   // 新根设置为黑色(此时的新根是父亲结点)
						grandFatherNode->rightChild->isRed = true; // 原爷爷结点
					}
					else
					{
						// 父亲是爷爷结点的右孩子
						if (parent->rightChild == point)
						{ // 新结点是父亲结点的右孩子
							// 左旋转
							RotateLeft(grandFatherNode);
						}
						else
						{
							// 先左旋后右旋
							RotateRightLeft(grandFatherNode);
						}
						// 变色
						grandFatherNode->isRed = false;			  // 新根设置为黑色(此时的新根是新结点)
						grandFatherNode->leftChild->isRed = true; // 原爷爷结点
					}
				}
				// 到这里根已经改变，要更新指向信息
				if (gff == nullptr)
				{
					root = grandFatherNode;
				}
				else if (sign == 1)
				{
					gff->leftChild = grandFatherNode;
				}
				else if (sign == 2)
				{
					gff->rightChild = grandFatherNode;
				}
				else
				{
					break;
				}
			}
			return;
		}

		// 获取兄弟结点指针
		RBNode<T> *getBrotherNode(RBNode<T> *p)
		{
			// 由调用者确定p->parent一定不为空
			if (p->parentNd->leftChild == p)
			{
				return p->parentNd->rightChild;
			}
			return p->parentNd->leftChild;
		}

		// 左旋转
		void RotateLeft(RBNode<T> *&point)
		{
			/*
						4                           6
					   / \           左旋          /  \
					  3   6         ======>       4    7
						 / \                     / \
						5   7                   3   5
			*/
			RBNode<T> *ptmproot = point;			 // ptmproot指向4
			point = ptmproot->rightChild;			 // point指向6
			point->parentNd = ptmproot->parentNd;	 // 令6的父亲指向4的父亲
			ptmproot->rightChild = point->leftChild; // 让4的右孩子不再指向6，而是指向5
			if (point->leftChild)
			{
				point->leftChild->parentNd = ptmproot; //设置5的父亲是4
			}
			point->leftChild = ptmproot; // 6的左孩子指向4
			ptmproot->parentNd = point;	 // 4的父亲为6
		}

		// 右旋转
		void RotateRight(RBNode<T> *&point)
		{
			/*
							 6                    4
						   /  \      右旋        /  \
						  4    7    ======>    3     6
						 / \                        / \
						3   5                      5   7
			*/
			RBNode<T> *ptmproot = point;			 // ptmproot指向6
			point = ptmproot->leftChild;			 // point指向4
			point->parentNd = ptmproot->parentNd;	 // 令4的父亲指向6的父亲
			ptmproot->leftChild = point->rightChild; // 让6的左孩子不再指向4，而是指向5
			if (point->rightChild)
			{
				point->rightChild->parentNd = ptmproot; // 设置5的父亲是6
			}
			point->rightChild = ptmproot; // 4的右孩子指向6
			ptmproot->parentNd = point;	  // 6的父亲为4
		}

		// 先左后右旋转
		void RotateLeftRight(RBNode<T> *&point)
		{
			/*
					6                    6                        5
				   / \     先左旋       /  \       再右旋         / \
				  4   7   =======>    5    7     ========>      4   6
				   \                 /                               \
					5               4                                 7
			*/
			RBNode<T> *ptmptroot = point;					  // 让ptmproot指向6
			RBNode<T> *ptmptroot_left = ptmptroot->leftChild; // 让ptmproot_l指向4
			point = ptmptroot_left->rightChild;				  // 让point指向5
			RBNode<T> *originParent = ptmptroot->parentNd;	  // 把6的父亲结点保存起来，后面要用
			// 先左旋转
			ptmptroot->leftChild = point; // 6的左孩子指向5，其实后面会覆盖的，这个赋值其实没用，为了代码清晰留着
			point->parentNd = ptmptroot;  // 5的父亲指向6，上行没用导致这行也没用

			ptmptroot_left->rightChild = point->leftChild; // 4的右孩子指向5的左孩子
			if (point->leftChild)
			{
				point->leftChild->parentNd = ptmptroot_left; // 5的左孩子的父亲为4
			}
			point->leftChild = ptmptroot_left; // 5的左孩子指向4
			ptmptroot_left->parentNd = point;

			// 再右旋转
			ptmptroot->leftChild = point->rightChild; // 6的左孩子指向5的右孩子
			if (point->rightChild)
			{
				point->rightChild->parentNd = ptmptroot; // 5的右孩子的父亲指向6
			}
			point->rightChild = ptmptroot; // 5的右孩子指向6
			ptmptroot->parentNd = point;   // 6的父亲指向5

			//处理根节点的父亲指向
			point->parentNd = originParent;
		}

		// 先右后左旋转
		void RotateRightLeft(RBNode<T> *&point)
		{
			/*
						6                        6                             7
					   / \       先右旋         /  \		   再左旋          / \
					  4   8     =======>      4    7         ========>       6   8
						 /                          \                       /
						7                            8                     4
			*/

			RBNode<T> *ptmproot = point;					  // ptmproot指向6
			RBNode<T> *ptmproot_right = ptmproot->rightChild; // 让ptmproot_right指向8
			point = ptmproot_right->leftChild;				  //让point指向7
			RBNode<T> *originParent = ptmproot->parentNd;	  // 把6的父亲保存起来

			// 先右旋转
			ptmproot->rightChild = point; // 6的右孩子指向7，其实后面会覆盖，为了清晰，留着
			point->parentNd = ptmproot;	  // 7的父亲指针指向6，上行没用导致本行没用

			ptmproot_right->leftChild = point->rightChild; // 8的左孩子指向7的右孩子
			if (point->rightChild)
			{
				point->rightChild->parentNd = ptmproot_right; // 7的右孩子的父亲指向7
			}
			point->rightChild = ptmproot_right; // 7的右孩子指向8
			ptmproot_right->parentNd = point;	// 8的父亲指向7

			// 再左旋转
			ptmproot->rightChild = point->leftChild; // 6的右孩子指向7的左孩子
			if (point->leftChild)
			{
				point->leftChild->parentNd = ptmproot; // 7的左孩子的父亲指向6
			}
			point->leftChild = ptmproot; // 7的左孩子指向6
			ptmproot->parentNd = point;	 // 6的父亲指向7

			//处理根节点的父亲指向
			point->parentNd = originParent;
		}

	private:
		void ReleaseNode(RBNode<T> *pnode)
		{
			if (pnode != nullptr)
			{
				ReleaseNode(pnode->leftChild);
				ReleaseNode(pnode->rightChild);
			}
			delete pnode;
		}

		RBNode<T> *root;
	};
}

int main()
{
	cout << "1111" << endl;
	return 0;
}
