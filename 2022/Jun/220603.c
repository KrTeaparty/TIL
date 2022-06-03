#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <stdbool.h>
#include <math.h>

#define t 9
#define max_children (2*t)
#define insert_cnt 500000
#define del_cnt 50000
#define max_keys ((2*t)-1)
#define min_keys t-1

typedef struct BTreeNode{
    bool leaf;
    int key[max_keys];
    int key_cnt;
    struct BTreeNode* child[max_children];
    int child_cnt;
} BTreeNode;

void Test();
BTreeNode* InitNode(int val);
BTreeNode* Split(int idx, BTreeNode* node, BTreeNode* parent);
BTreeNode* Insert_Node(int parent_pos, int val, BTreeNode* node, BTreeNode* parent);
void Insert(int val);
void Merge_Node(BTreeNode* parent_node, int idx, int merge_idx);
void B_from_Left(BTreeNode* parent_node, int cur_idx);
void B_from_Right(BTreeNode* parent_node, int cur_idx);
void Balancing(BTreeNode* node, int child_pos);
int Merge_Child(BTreeNode* parent_node, int cur_idx);
int Find_Predecessor(BTreeNode* cur_node);
int Override_Predecessor(BTreeNode* parent_node, int idx_search);
int Find_Successor(BTreeNode* cur_node);
int Override_Successor(BTreeNode* parent_node, int idx_search);
void Del_Inner_Node(BTreeNode* cur_node, int cur_idx);
int Del_Val(int val, BTreeNode* node);
void Delete(BTreeNode* node, int val);

BTreeNode* root;

int main(void) {
    Test();
    return 0;
}

void Test() {
    LARGE_INTEGER tickPerSecond;
    LARGE_INTEGER startTick, endTick;
    double dTime;

    QueryPerformanceFrequency(&tickPerSecond);

    QueryPerformanceCounter(&startTick);
    for(int i=0; i<insert_cnt; i++) {
        Insert(rand() % 10000);
    }
    printf("Inserting Finished\n");
    for(int i=0; i<del_cnt; i++) {
        Delete(root, rand() % 10000);
    }
    printf("Deleting Finished\n");
    QueryPerformanceCounter(&endTick);
    
    dTime = (double)(endTick.QuadPart - startTick.QuadPart) / tickPerSecond.QuadPart;
    printf("Result of t=%d : %lf", t, dTime);
}

BTreeNode* InitNode(int val){ 
    BTreeNode* newNode;
    newNode = (BTreeNode*)malloc(sizeof(BTreeNode));
    newNode->leaf = false;
    newNode->key[0] = val;
    newNode->key_cnt = 1;
    newNode->child_cnt = 0;
    return newNode;
}

BTreeNode* Split(int idx, BTreeNode* node, BTreeNode* parent){
    int median;
    median = node->key_cnt / 2;
    BTreeNode* right_node;

    right_node = (BTreeNode*)malloc(sizeof(BTreeNode));
    right_node->leaf = node->leaf;
    right_node->key_cnt = 0;
    right_node->child_cnt = 0;
    
    int loop = node->key_cnt;
    for(int i = median+1; i< loop; i++){
        right_node->key[i-(median+1)] = node->key[i];
        right_node->key_cnt++;
        node->key_cnt--;
    }

    if (!node->leaf){
        loop = node->child_cnt;
        for (int i = median+1; i < loop; i++ ) {
            right_node->child[i-(median+1)] = node->child[i];
            right_node->child_cnt++;
            node->child_cnt--;
        }
    }
    
    if (node==root) {
        BTreeNode* new_parent_node;
        new_parent_node = InitNode(node->key[median]);
        node->key_cnt--;
        new_parent_node->child[0] = node;
        new_parent_node->child[1] = right_node;

        new_parent_node->child_cnt = 2;
        return new_parent_node; 
    }
    else {
        for (int i= parent->key_cnt; i> idx; i--){
            parent->key[i] = parent->key[i-1];
            parent->child[i+1] = parent->child[i];
        }

        parent->key[idx]= node->key[median];
        parent->key_cnt++;
        node->key_cnt--;
        parent->child[idx+1] = right_node;
        parent->child_cnt+=1;
    }
    return node;
}


BTreeNode* Insert_Node(int parent_pos, int val, BTreeNode* node, BTreeNode* parent) {
    int idx;
    for (idx=0; idx < node->key_cnt; idx++ ) {
        if (val == node->key[idx]){
            return node;
        }
        else if (val< node->key[idx]){
            break;
        }
    }
    if (!node->leaf) {
        node->child[idx] = Insert_Node(idx, val, node->child[idx] ,node);
        if (node->key_cnt == max_keys){
            node = Split(parent_pos, node,parent);
        }
    }
    else {
        for (int i = node->key_cnt; i > idx; i--) {
            node->key[i] = node->key[i-1];
            node->child[i+1] = node->child[i];
        }

        node->key[idx] = val;
        node->key_cnt++;
        if (node->key_cnt == max_keys){
            node = Split(parent_pos, node,parent);
        }
    }   
    return node;

}

void Insert(int val){
    if (!root){
        root = InitNode(val);
        root->leaf = true ;
        return;
    }
    else{
        root = Insert_Node(0,val,root,root);
    }
}

void Merge_Node(struct BTreeNode* par_node, int node_pos, int mer_pos){
    
    int merge_idx = par_node->child[mer_pos]->key_cnt;
    par_node->child[mer_pos]->key[merge_idx]= par_node->key[mer_pos];
    par_node->child[mer_pos]->key_cnt++;

    for(int i=0; i<par_node->child[node_pos]->key_cnt; i++){
        par_node->child[mer_pos]->key[merge_idx+1+i] = par_node->child[node_pos]->key[i];
        par_node->child[mer_pos]->key_cnt++;
    }

    int merge_childidx = par_node->child[mer_pos]->child_cnt;
    for (int i=0; i<par_node->child[node_pos]->child_cnt; i++){
        par_node->child[mer_pos]->child[merge_childidx+i] = par_node->child[node_pos]->child[i];
        par_node->child[mer_pos]->child_cnt++;
    }

    free(par_node->child[node_pos]);

    for(int i = mer_pos; i < (par_node->key_cnt)-1; i++) {
        par_node->key[i] = par_node->key[i+1];
    }
    par_node->key_cnt--;

    for(int i = mer_pos+1; i<(par_node->child_cnt)-1; i++){
        par_node->child[i] = par_node->child[i+1];
    }
    par_node->child_cnt--;
}

void B_from_Left(BTreeNode* parent_node, int cur_idx){
    int t_idx = 0;

    for (int i=0; i< parent_node->child[cur_idx]->key_cnt; i++){ 
        parent_node->child[cur_idx]->key[i+1] = parent_node->child[cur_idx]->key[i];
    }
    parent_node->child[cur_idx]->key[t_idx] = parent_node->key[cur_idx-1];
    parent_node->child[cur_idx]->key_cnt++;

    int sib_idx= (parent_node->child[cur_idx-1]->key_cnt) -1;
    parent_node->key[cur_idx-1] = parent_node->child[cur_idx-1]->key[sib_idx];
    parent_node->child[cur_idx-1]->key_cnt--;

    if (parent_node->child[cur_idx-1]->child_cnt>0){
        int tc_idx = (parent_node->child[cur_idx-1]->child_cnt)-1; 
        for (int i = parent_node->child[cur_idx]->child_cnt; i > 0; i--){
            parent_node->child[cur_idx]->child[i] = parent_node->child[cur_idx]->child[i-1];
        }

        parent_node->child[cur_idx]->child[0] = parent_node->child[cur_idx-1]->child[tc_idx];
        parent_node->child[cur_idx]->child_cnt++;

        parent_node->child[cur_idx-1]->child_cnt--;
    }
}

void B_from_Right(BTreeNode* parent_node, int cur_idx){
    int t_idx = parent_node->child[cur_idx]->key_cnt;
    parent_node->child[cur_idx]->key[t_idx] = parent_node->key[cur_idx];
    parent_node->child[cur_idx]->key_cnt++;

    int sib_idx= 0;
    parent_node->key[cur_idx] = parent_node->child[cur_idx+1]->key[sib_idx];

    for (int i=0; i< (parent_node->child[cur_idx+1]->key_cnt)-1; i++){ 
        parent_node->child[cur_idx+1]->key[i] = parent_node->child[cur_idx+1]->key[i+1];
    }
    parent_node->child[cur_idx+1]->key_cnt--;

    int idx_from_sib = 0;
    if (parent_node->child[cur_idx+1]->child_cnt>0){
        int tc_idx = parent_node->child[cur_idx]->child_cnt;
        parent_node->child[cur_idx]->child[tc_idx] = parent_node->child[cur_idx+1]->child[idx_from_sib];
        parent_node->child[cur_idx]->child_cnt++;

        for (int i = 0; i<parent_node->child[cur_idx+1]->child_cnt-1; i++){
            parent_node->child[cur_idx+1]->child[i] = parent_node->child[cur_idx+1]->child[i+1];
        }
        parent_node->child[cur_idx+1]->child_cnt--;
    }
}

void Balancing(BTreeNode* node, int child_pos){
    if (child_pos==0){
        if (node->child[child_pos+1]->key_cnt > min_keys){
            B_from_Right(node,child_pos);
        }
        else{
            Merge_Node(node,child_pos+1,child_pos);
        }
        return;
    }

    else if (child_pos == (node->key_cnt)){
        if (node->child[child_pos-1]->key_cnt > min_keys){
            B_from_Left(node,child_pos);
        }
        else{
            Merge_Node(node,child_pos,child_pos-1); 
        }
        return;
    }
    else{
        if (node->child[child_pos-1]->key_cnt > min_keys){
            B_from_Left(node,child_pos); 
        }
        else if (node->child[child_pos+1]->key_cnt > min_keys){
            B_from_Right(node,child_pos);
        }
        else{
            Merge_Node(node,child_pos,child_pos-1);
        }
        return;
    }
}

int Merge_Child(BTreeNode* parent_node, int cur_idx){
    int mergeidx = parent_node->child[cur_idx]->key_cnt;
    int parent_node_val = parent_node->key[cur_idx];
    parent_node->child[cur_idx]->key[mergeidx] = parent_node->key[cur_idx]; 
    parent_node->child[cur_idx]->key_cnt++;

    for (int i=0; i < parent_node->child[cur_idx+1]->key_cnt; i++){
        parent_node->child[cur_idx]->key[mergeidx+1+i] = parent_node->child[cur_idx+1]->key[i];
        parent_node->child[cur_idx]->key_cnt++;
    }

    for (int i=0; i< parent_node->child[cur_idx+1]->child_cnt; i++){
        parent_node->child[cur_idx]->child[mergeidx+1+i] = parent_node->child[cur_idx+1]->child[i];
        parent_node->child[cur_idx]->child_cnt++;
    }

    for (int i= cur_idx; i<parent_node->key_cnt; i++){
        parent_node->key[i] = parent_node->key[i+1];
        parent_node->key_cnt--;
    }
    for (int i = cur_idx+1; i < parent_node->child_cnt; i++){
        parent_node->child[i]= parent_node->child[i+1];
        parent_node->child_cnt--;
    }
    return parent_node_val;

}


int Find_Predecessor(BTreeNode* cur_node){
    int predecessor;
    if (cur_node->leaf){
        return cur_node->key[cur_node->key_cnt-1];
    }
    return Find_Predecessor(cur_node->child[(cur_node->child_cnt)-1]);
}

int Override_Predecessor(BTreeNode* parent_node, int idx_search){
    int predecessor = Find_Predecessor(parent_node->child[idx_search]);
    parent_node->key[idx_search] = predecessor;
    return predecessor;
}

int Find_Successor(BTreeNode* cur_node){
    int successor;
    if (cur_node->leaf){
        return cur_node->key[0];
    }
    return Find_Successor(cur_node->child[0]);
}

int Override_Successor(BTreeNode* parent_node, int idx_search){
    int successor = Find_Successor(parent_node->child[idx_search+1]);
    parent_node->key[idx_search] = successor;
    return successor;
}

void Del_Inner_Node(BTreeNode* cur_node, int cur_idx){
    int cessor = 0;
    int del_merge = 0;

    if (cur_node->child[cur_idx]->key_cnt >= cur_node->child[cur_idx+1]->key_cnt){ 
        if(cur_node->child[cur_idx]->key_cnt > min_keys){
            cessor = Override_Predecessor(cur_node,cur_idx);
            Del_Val(cessor,cur_node->child[cur_idx]);
        }
        else{
            del_merge = Merge_Child(cur_node,cur_idx);
            Del_Val(del_merge, cur_node->child[cur_idx]);
        }
    }
    else {
        if (cur_node->child[cur_idx+1]->key_cnt > min_keys){
            cessor = Override_Successor(cur_node,cur_idx);
            Del_Val(cessor,cur_node->child[cur_idx+1]);
        }
        else{
            del_merge = Merge_Child(cur_node,cur_idx);
            Del_Val(del_merge, cur_node->child[cur_idx]);
        }

    }

}

int Del_Val(int val, BTreeNode* node){ 
    int idx;
    int flag = false;
    for (idx=0; idx < node->key_cnt; idx++){
        if (val == node->key[idx]){
            flag = true;
            break;
        }
        else if (val< node->key[idx]){
            break;
        }
    }
    if (flag){
        if (node->leaf){
            for (int i = idx; i<node->key_cnt; i++){
                node->key[i] = node->key[i+1];
            }
            node->key_cnt--;
        }
        else {
                Del_Inner_Node(node,idx);
        }
        return flag;
    }
    else {
        if (node->leaf) {
            return flag;
        }
        else{
            flag = Del_Val(val, node->child[idx]);
        }
    }
    if (node->child[idx]->key_cnt < min_keys){
        Balancing(node,idx);
    }

    return flag;    
}


void Delete(BTreeNode* node, int val){
    if (!node){
        return;
    }
    int flag = Del_Val(val,node);
    if (!flag){
        return;
    }
    if (node->key_cnt == 0){
        node = node->child[0];
    }
    root = node;
}