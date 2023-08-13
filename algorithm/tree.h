
void trace_tree_v1(tree * n)
{
	if (NULL == n) {
		return ;
	}

	if (NULL != n->left) {
		trace_tree(n->left);
	}
	printf("%s\n", n->name);

	if(NULL != n->right) {
		trace_tree(n->right);
	}
}

// 中序
void trace_tree_v2(tree * n)
{

	Stack s;
	tree * p = n;

	for (;!s.empty() || NULL != p;) {

		if(NULL != p) {
			s.push(p);		
			p = p->left;
		} else if(!s.empty()) {
			p=s.pop();
			p->print();
			p = p->right;
		}
	}
}

// 后序
void trace_tree_v2(tree * n)
{

	Stack s;
	tree * p = n;
    tree* last_p = NULL;

	for (;!s.empty() || NULL != p;) {

		if(NULL != p) {
			s.push(p);		
			p = p->left;
		} else if(!s.empty()) {
			p=s.top();
            if (NULL != p->right && last_p != p->right){
                p = p->right;
                continue;
            }
			p->print();
            last_p = p;
            p.pop();
		}
	}
}

// 前序
void trace_tree_v2(tree * n)
{

	Stack s;
	tree * p = n;
    s.push(p);

	for (;!s.empty() || NULL != p;) {

        p = s.pop();

        if(NULL != p) {
            p->print();
        }
        if (NULL != p->right) {
            s.push(p->rhght);
        }
        if (NULL != p->left) {
            s.push(p->left);
        }
    }
}

// 大小
int size(tree* n)
{
    if (NULL == s) {
        return 0;
    }
    Stack s;
    tree* p = n;
    s.push(p);
    for (;!s.empty();) {
        p = s.pop();
        s ++;
        if (NULL != p->left) {
            s.push(p->left);
        }

        if (NULL != p->right) {
            s.push(p->right);
        }
    }

    return s;
}

// 最大深度
struct max_d_t {
    tree* n;
    int level;
};
int max_depth(tree* n) 
{
    if (NULL== n) {
        return 0;
    }
    Stack s;
    tree* p = n;
    s.push(max_d_t(p, 1));
    int md = 0;
    for (;!s.empty();) {
        max_d_t* p = s.pop();
        md = p->level > md ? p->level : md;
        if (NULL != p->n->left) {
            s.push(max_d_t(p->->left, p->level + 1));
        }

        if (NULL != p->n->right) {
            s.push(max_d_t(p->->right, p->level + 1));
        }
    }

    return md;
}

// 针对每个节点，交换左节点和右节点
int mirror(tree* n)
{
    if (NULL == n) {
        return ;
    }
    tree* p = n;
    Stack s;
    s.push(p);
    for (;!s.empty();) {
        p = s.pop();
        if (NULL != p->left && NULL != p->right) {
            swap(p->left, p->right);
        }
        if (NULL != p->left) {
            s.push(p->left);
        }
        if (NULL != p->right) {
            s.push(p->right);
        }
    }

    return 0;
}
// 判断是否查找树
bool is_bst(tree* n)
{
    if (NULL== n) {
        true;
    }
    bool is_left_bst = true; 
    int left_val = MIN_VALUE;
    bool is_right_bst = true; 
    int right_val = MAX_VALUE;
    if (NULL != n->left) {
        is_left_bst = is_bst(n->left);
        left_val = n->left->val;
    }
    if (NULL != n->rihgt) {
        is_right_bst = is_bst(n->right);
        right_val = n->riht->val;
    }
    return is_left_bst && is_right_bst && (left_val < n->val && n->val < right_val);
}

struct node_level_t {
    tree* node;
    int level;
};

// 蛇形遍历
int print_node(tree* n)
{
    if (NULL == n) {
        return -1;
    }
    // 分树层级存储节点
    Stack s[128];
    tree* p = n;
    node_level_t nl;
    int global_level = 1;
    s[global_level].push(node_level_t(p, 1));
    for (;!s[global_level].empty() && global_level <= 128;) {
        nl = s[global_level].pop();
        nl->print();
        if (nl->level % 2 == 0) {
            if (NULL != nl->node->left) {
                s.push(node_level_t(nl->node->left, nl->level + 1));
            }

            if (NULL != nl->node->right) {
                s.push(node_level_t(nl->node->right, nl->level + 1));
            }
        } else {
            if (NULL != nl->node->right) {
                s.push(node_level_t(nl->node->right, nl->level + 1));
            }
            if (NULL != nl->node->left) {
                s.push(node_level_t(nl->node->left, nl->level + 1));
            }
        }
        //逐层遍历树节点
        if (s[global_level].empty()) {
            ++ global_level;
        } 
    }
    return 0;
}


