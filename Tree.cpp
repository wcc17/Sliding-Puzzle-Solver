////
//// Created by Christian Curry on 4/20/16.
////
//
//#include "Tree.h"
//Tree::Tree() {
//    root = 0;
//}
//
//void Tree::destroyTree(Node *leaf)
//{
//    if(leaf != 0)
//    {
//        destroyTree(leaf->left);
//        destroyTree(leaf->right);
//        delete leaf;
//    }
//}
//
//void Tree::insert(int key, int g, int h, Node *leaf)
//{
//    if(key< leaf->keyValue)
//    {
//        if(leaf->left != 0) {
//            insert(key, g, h, leaf->left);
//        }
//        else
//        {
//            leaf->left = new Node;
//
//            leaf->left->keyValue = key;
//            leaf->g = g;
//            leaf->h = h;
//
//            leaf->left->left=0;    //Sets the left child of the child node to null
//            leaf->left->right=0;   //Sets the right child of the child node to null
//        }
//    }
//    else if(key >= leaf->keyValue)
//    {
//        if(leaf->right != 0) {
//            insert(key, g, h, leaf->right);
//        }
//        else
//        {
//            leaf->right = new Node;
//
//            leaf->right->keyValue = key;
//            leaf->g = g;
//            leaf->h = h;
//
//            leaf->right->left=0;  //Sets the left child of the child node to null
//            leaf->right->right=0; //Sets the right child of the child node to null
//        }
//    }
//}
//
//Node *Tree::search(int key, Node *leaf)
//{
//    if(leaf!=0)
//    {
//        if(key == leaf->keyValue)
//            return leaf;
//        if(key < leaf->keyValue)
//            return search(key, leaf->left);
//        else
//            return search(key, leaf->right);
//    }
//    else return 0;
//}
//
//void Tree::insert(int g, int h)
//{
//    int key = g + h;
//
//    if(root != 0) {
//        insert(key, g, h, root);
//    }
//    else
//    {
//        root = new Node;
//
//        root->keyValue = key;
//        root->g = g;
//        root->h = h;
//
//        root->left = 0;
//        root->right = 0;
//    }
//}
//
//Node *Tree::search(int key)
//{
//    return search(key, root);
//}
//
//void Tree::destroyTree()
//{
//    destroyTree(root);
//}