
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
