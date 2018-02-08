//
// Created by yangzheng on 17-6-14.
//

#ifndef GST_VIDEO_PLAYER_VIDEO_CONTAINER_H
#define GST_VIDEO_PLAYER_VIDEO_CONTAINER_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <gst/gst.h>

typedef struct _elem_branch_pointer elem_branch_pointer;
typedef struct _pip_elements_node pip_elements_node;
typedef struct _video_playbin video_playbin;

struct _elem_branch_pointer{
    pip_elements_node* next_elem;
    struct _elem_branch_pointer *next;
};

struct _pip_elements_node{
    GstElement *video_elem;
    char elem_id[50];
    int n_branch;
    elem_branch_pointer *p_branch;
};

struct _video_playbin{
    GstElement *pipline;
    pip_elements_node *elements_tree;
};

pip_elements_node* init_element_node();
elem_branch_pointer* init_branch_node();
video_playbin* init_video_playbin();
pip_elements_node* insert_elem_to_pip(pip_elements_node* p_pen, GstElement* elem, const char* elem_id);
int insert_branch_to_elem(elem_branch_pointer* branch_pointer, pip_elements_node* inserted_elem);

#endif //GST_VIDEO_PLAYER_VIDEO_CONTAINER_H
