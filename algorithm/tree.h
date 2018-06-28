
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
