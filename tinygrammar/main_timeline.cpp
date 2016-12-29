//
//  main.cpp
//  tinygrammar
//
//  Created by tangles on 6/8/16.
//  Copyright © 2016 visgraph. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include "time_manager.h"
#include "expansion_manager.h"
#include "input_figures.h"
#include "svg.h"


void printTimeLine(Grammar* g, TimeManager::TimeLine* t){
    int scale = 80;
    std::cout << std::endl;
    std::cout << " ~~~ Printing Timeline ~~~ " << std::endl;
    auto k = 0;
    for (auto&& ntl : t->timelines){
        std::cout << (k++) << " " << mapping_to_tag(g, ntl->node->content->shapes[0]->tag) << "\t|";
        auto ntl_duration = ntl->duration;
        char slice_char = '-';
        for (auto&& s : ntl->slices){
            int num = (int)(scale * (s->duration / ntl_duration));
            std::cout << "{";
            std::cout << std::string(num, slice_char);
            std::cout << "}";
        }
        std::cout << "|" << std::endl;
        
        std::cout << "\t\t|";
        slice_char = ' ';
        for (auto&& s : ntl->slices){
            int num = (int)(scale * (s->duration / ntl_duration));
            stringstream ss;
            ss << mapping_to_tag(g, s->ts_tag) << " (" << s->duration << ")";
            int padding = num - (int)ss.str().length();
            
            if (padding > 0){
                std::cout << "{" << std::string(padding / 2, slice_char);
                
                std::cout << ss.str();
                
                if (padding % 2 == 0) std::cout << std::string(padding / 2, slice_char);
                else std::cout << std::string(padding / 2 + 1, slice_char);
            }
            else {
                padding = ym_clamp(num - (int)(mapping_to_tag(g, s->ts_tag).length()), 0, 10);
                std::cout << "{" << std::string(padding / 2, slice_char);
                
                std::cout << mapping_to_tag(g, s->ts_tag);
                
                if (padding % 2 == 0) std::cout << std::string(padding / 2, slice_char);
                else std::cout << std::string(padding / 2 + 1, slice_char);
            }
            
            std::cout << "}";
        }
        std::cout << "|" << std::endl;
    }
    std::cout << std::endl;
}

int main(int argc, const char * argv[]) {
    
    auto em = (HistoryAnim*)(make_history(animation_history));
    auto grammar = get_grammar(grammar_filename);
    auto tree = initialize_tree(grammar, 3, 5, "resources/svg/woman_tagged.svg");
    
    auto init_step = matching_init();
    auto init_shapes = init_step->op(ShapeGroup(), init_step->produced_tags, init_step->parameters, grammar->rn, nullptr, nullptr, tree);
    auto init_partition = PartitionShapeGroup();
    init_partition.added = init_shapes;
    init_partition.remainder = ShapeGroup();
    init_partition.match = ShapeGroup();
    
    update_history(em, init_partition, init_step, tree);
    
    ((ExpansionAnim*)(em->history.back()))->tree = tree;
    
    printTimeLine(grammar, ((ExpansionAnim*)(em->history.back()))->timeline);
    
    while (expand(em)){
        printf("Expanding... \n");
        printTimeLine(grammar, ((ExpansionAnim*)(em->history.back()))->timeline);
    };
    
    auto last_exp = ((ExpansionAnim*)(em->history.back()));
    auto duration = last_exp->timeline->duration;
    double frame_rate = grammar->framerate;
    double frame_step = 1.0 / frame_rate;
    auto k = 1;
    
    stringstream ss;
    ss << std::setfill('0') << std::setw(3) << 0;
    auto sca_svg = 1.0;
    auto bbox = bounds_polygons(make_vector(last_exp->tree->leaves, [&](CSGTree::LeafNode* node){return node->content->shapes[0]->poly;}));
    bbox = {ym_rcenter(bbox) - ym_rsize(bbox) * sca_svg / 2.0, ym_rcenter(bbox) + ym_rsize(bbox) * sca_svg / 2.0};
    auto size = ym_rsize(bbox);
    auto center = ym_rcenter(bbox);
    

    TimeManager::AnimateTimeLine(last_exp->timeline, last_exp->tree, 0.0, frame_step);
    save_svg(last_exp->tree, {(int)size.x, (int)size.y}, {-center.x/2.0, -center.y/2.0}, {sca_svg, sca_svg}, ss.str());
    
    for (auto i = frame_step; (i - duration) <= EPS_2; i = i + frame_step){
        if (IS_DEBUG) printf("Animating frame %d\n", k);
        else  printf("#%d...", k);
        TimeManager::AnimateTimeLine(last_exp->timeline, last_exp->tree, ym_clamp(i, 0.0, duration), frame_step);
        stringstream ss1;
        ss1 << std::setfill('0') << std::setw(3) << k;
        save_svg(last_exp->tree, {(int)size.x, (int)size.y}, {-center.x/2.0, -center.y/2.0}, {sca_svg, sca_svg}, ss1.str());
        k++;
    }
    
    // !!!!!!!!!!!!!!!! TODO IMPORTANT !!!!!!!!!!!!!!!!!!!
    // - OPTIMIZATION OF ANIMATION ROUTINE -> ONE TREE UPDATE ONCE *ALL* LEAVES ARE UPDATED
    // - fix offset and document size of saved svg
    // - test animation with big svg
    // - consider change resolution of imported svg
    
    //convert -density 40 -resize 500x500 svg/*.svg -set filename:base "%[base]" png/"%[filename:base].png"
    //convert -delay 1/8 -loop 0 png/*.png animated4.gif
    
    printf("end main\n");
}
