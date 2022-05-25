#include "bsp.hpp"
#include "node.hpp"

Bsp::Bsp(Map &map) : map_(map), root(nullptr) {
}

Bsp::~Bsp() {
    if (root)
        delete root;
}

void Bsp::build(Renderer &renderer) {
    auto segs = create_segs();

    Polyf poly;
    poly.add(Vec2f(map_.bounds().min().x, map_.bounds().min().y));
    poly.add(Vec2f(map_.bounds().min().x, map_.bounds().max().y));
    poly.add(Vec2f(map_.bounds().max().x, map_.bounds().max().y));
    poly.add(Vec2f(map_.bounds().max().x, map_.bounds().min().y));

    root = new Node(segs, poly, renderer);
}

void Bsp::save() {
    // Dont save if no nodes have been built
    if (!root)
        return;

    // Recursively process the nodes
    process_linedefs();
    process_node(root);

    // Replace the lumps
    map_.replace_vertices(&vertices[0], vertices.size());
    map_.replace_linedefs(&linedefs[0], linedefs.size());
    map_.replace_segs(&segs[0], segs.size());
    map_.replace_ssectors(&ssectors[0], ssectors.size());
    map_.replace_nodes(&nodes[0], nodes.size());
}

std::vector<Seg> Bsp::create_segs() {
    std::vector<Seg> segs;

    auto vertices = map_.get_vertices();
    auto linedefs = map_.get_linedefs();

    for (auto i = 0; i < map_.num_linedefs(); i++, linedefs++) {
        auto p1 = Vec2f(vertices[linedefs->start].x, vertices[linedefs->start].y);
        auto p2 = Vec2f(vertices[linedefs->end].x, vertices[linedefs->end].y);

        segs.push_back(Seg(p1, p2, false, 0, i));

        // Two sided
        if (linedefs->flags & 0b100)
            segs.push_back(Seg(p2, p1, true, 0, i));
    }

    return segs;
}

std::size_t Bsp::unique_vertex(int x, int y) {
    // First check to see if the vertex already exists
    for (auto i = 0; i < vertices.size(); i++) {
        if (vertices[i].x == x && vertices[i].y == y)
            return i;
    }

    // Otherwise create it
    Map::Vertex vertex;
    vertex.x = x;
    vertex.y = y;
    vertices.push_back(vertex);

    return vertices.size() - 1;
}

// Not exactly nessary, but still might as well have this, as to insure unique vertices
void Bsp::process_linedefs() {
    auto vertices = map_.get_vertices();
    auto linedefs = map_.get_linedefs();

    for (auto i = 0; i < map_.num_linedefs(); i++, linedefs++) {
        auto linedef = *linedefs;
        linedef.start = unique_vertex(vertices[linedefs->start].x, vertices[linedefs->start].y);
        linedef.end   = unique_vertex(vertices[linedefs->end].x, vertices[linedefs->end].y);

        this->linedefs.push_back(linedef);
    }
}

int Bsp::process_ssector(const Node *node) {
    Map::SSector ssector;
    ssector.count = node->segs().size();
    ssector.first = segs.size();

    // Process the segs
    for (const auto &seg : node->segs()) {
        Map::Seg map_seg;

        map_seg.start   = unique_vertex(seg.p1().x, seg.p1().y);
        map_seg.end     = unique_vertex(seg.p2().x, seg.p2().y);
        map_seg.angle   = seg.angle();
        map_seg.linedef = seg.linedef();
        map_seg.dir     = seg.side();
        map_seg.offset  = seg.offset();

        segs.push_back(map_seg);
    }

    ssectors.push_back(ssector);

    return ssectors.size() - 1;
}

int Bsp::process_node(const Node *node) {
    // If the node is a leaf, create sub sector
    if (node->leaf())
        return process_ssector(node) | (1 << 15); // Sub sector flag

    Map::Node map_node;
    map_node.x  = node->splitter().p.x;
    map_node.y  = node->splitter().p.y;
    map_node.dx = node->splitter().dx;
    map_node.dy = node->splitter().dy;

    // Process the children nodes
    map_node.child[0] = process_node(node->left());
    map_node.child[1] = process_node(node->right());

    // Left bounding box
    map_node.lbounds[0] = node->left()->bounds().max().y; // Top
    map_node.lbounds[1] = node->left()->bounds().min().y; // Bottom
    map_node.lbounds[2] = node->left()->bounds().min().x; // Left
    map_node.lbounds[3] = node->left()->bounds().max().x; // Right

    // Right bounding box
    map_node.rbounds[0] = node->right()->bounds().max().y; // Top
    map_node.rbounds[1] = node->right()->bounds().min().y; // Bottom
    map_node.rbounds[2] = node->right()->bounds().min().x; // Left
    map_node.rbounds[3] = node->right()->bounds().max().x; // Right

    nodes.push_back(map_node);

    return nodes.size() - 1;
}
