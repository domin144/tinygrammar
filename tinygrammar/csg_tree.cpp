#include "csg_tree.h"

CSGTree::Tree* CSGTree::InitTree() {
    return new Tree();
}

void CSGTree::AddNode(CSGTree::Tree* tree, CSGTree::LeafNode* node){
    tree->nodes.push_back(node);
    if (tree->root  == nullptr) { tree->root = node; }
}

void CSGTree::AddNode(CSGTree::Tree* tree, CSGTree::OpNode* node){
    tree->nodes.push_back(node);
    if (tree->root  == nullptr) { tree->root = node; }
    else if ((tree->root == node->child_left) || (tree->root == node->child_right)){ tree->root = node; }
}

CSGTree::LeafNode* CSGTree::AddShape(CSGTree::Tree* tree, AnimatedShape* shape){
    auto node = new CSGTree::LeafNode();
    auto nodeC = new CSGTree::NodeContent(shape);
    node->content = nodeC;
    CSGTree::AddNode(tree, node);
    return node;
}

CSGTree::OpNode* CSGTree::BuildResult(const vector<polygon2r>& shapes, CSGTree::Node* a, CSGTree::Node* b){
    auto result_shapes = vector<AnimatedShape*>();
    for (auto&& r : shapes){
        auto as = new AnimatedShape();
        as->poly = r;
        result_shapes.push_back(as);
    }
    
    auto node = new CSGTree::OpNode();
    node->child_left = a;
    node->child_right = b;
    node->content = new NodeContent(result_shapes);
    
    return node;
}

CSGTree::OpNode* CSGTree::Union(CSGTree::Tree* tree, CSGTree::Node* a, CSGTree::Node* b){
    vector<polygon2r> a_polys = make_vector(a->content->shapes, [&](AnimatedShape* shape){return shape->poly;});
    vector<polygon2r> b_polys = make_vector(b->content->shapes, [&](AnimatedShape* shape){return shape->poly;});
    auto result = union_polygons_clipper(a_polys, b_polys);
    auto res_node = CSGTree::BuildResult(result, a, b);
    res_node->op_type = union_op;
    a->parent = res_node;
    b->parent = res_node;
    CSGTree::AddNode(tree, res_node);
    return res_node;
}

CSGTree::OpNode* CSGTree::Difference(CSGTree::Tree* tree, CSGTree::Node* a, CSGTree::Node* b){
    vector<polygon2r> a_polys = make_vector(a->content->shapes, [&](AnimatedShape* shape){return shape->poly;});
    vector<polygon2r> b_polys = make_vector(b->content->shapes, [&](AnimatedShape* shape){return shape->poly;});
    auto result = subtract_polygons_clipper(a_polys, b_polys);
    auto res_node = CSGTree::BuildResult(result, a, b);
    res_node->op_type = difference_op;
    a->parent = res_node;
    b->parent = res_node;
    CSGTree::AddNode(tree, res_node);
    return res_node;
}

CSGTree::OpNode* CSGTree::Intersection(CSGTree::Tree* tree, CSGTree::Node* a, CSGTree::Node* b){
    vector<polygon2r> a_polys = make_vector(a->content->shapes, [&](AnimatedShape* shape){return shape->poly;});
    vector<polygon2r> b_polys = make_vector(b->content->shapes, [&](AnimatedShape* shape){return shape->poly;});
    auto result = intersect_polygons(a_polys, b_polys);
    auto res_node = CSGTree::BuildResult(result, a, b);
    res_node->op_type = intersection_op;
    a->parent = res_node;
    b->parent = res_node;
    CSGTree::AddNode(tree, res_node);
    return res_node;
}
