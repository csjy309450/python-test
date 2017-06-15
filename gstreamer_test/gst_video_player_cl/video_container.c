//
// Created by yangzheng on 17-6-14.
//
#include "video_container.h"

pip_elements_node* init_element_node(){
    pip_elements_node* pelem=(pip_elements_node*)malloc(sizeof(pip_elements_node));
    if(NULL==pelem){
        printf("error: create pip_elements_node failed.");
        return NULL;
    }
    pelem->video_elem=NULL;
    strcpy(pelem->elem_id,"");
    pelem->n_branch=0;
    pelem->p_branch=NULL;
    return pelem;
}

elem_branch_pointer* init_branch_node(){
    elem_branch_pointer* ebp=(elem_branch_pointer*)malloc(sizeof(elem_branch_pointer));
    if(NULL==ebp){
        printf("error: create elem_branch_pointer failed.");
        return NULL;
    }
    ebp->next_elem=NULL;
    ebp->next=NULL;
    return ebp;
}

video_playbin* init_video_playbin(){
    video_playbin* playbin=(video_playbin*)malloc(sizeof(video_playbin));
    if(NULL==playbin){
        printf("error: create video_playbin failed.");
        return NULL;
    }
    playbin->pipline=NULL;
    playbin->elements_tree=NULL;
    return playbin;
}

int insert_branch_to_elem(elem_branch_pointer* branch_pointer, pip_elements_node* inserted_elem){
    if(NULL==branch_pointer||NULL==inserted_elem){
        printf("error: parameters can't be NULL.");
        return -1;
    }
    elem_branch_pointer* ebp=init_branch_node(), *p_work;
    if(NULL==ebp){
        return -1;
    }
    p_work = branch_pointer;
    while(NULL!=p_work->next){
        p_work=p_work->next;
    }
    if(NULL==p_work){
        printf("error: can't find parent_elem.");
        free(ebp);
        return -1;
    }
    ebp->next=p_work->next;
    ebp->next_elem=inserted_elem;
    return 1;
}

pip_elements_node* insert_elem_to_pip(pip_elements_node* p_pen, GstElement* elem, const char* elem_id){
    if(NULL==p_pen||NULL==elem){
        printf("error: parameters can't be NULL.");
        return NULL;
    }
    pip_elements_node* p_elem=init_element_node();
    if(NULL==p_elem){
        return NULL;
    }
    p_elem->video_elem=elem;
    strcpy(p_elem->elem_id,elem_id);
    p_elem->n_branch+=1;
    if(-1==insert_branch_to_elem(p_pen->p_branch,p_elem)){
        free(p_elem);
        return NULL;
    }
    return p_elem;
}
