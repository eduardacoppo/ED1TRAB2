#include "tree.h"
#include "bitmap.h"
#include <stdlib.h>
#include <stdio.h>

struct branch {
	Branch *left, *right, *before;
	void *data;
};

//Função privada, serve para testar o getWay
void checkWay(bitmap bm, Branch *root, Branch *bTest);

Branch* tree_newBranch(void *data) {
	Branch *branch = malloc(sizeof(Branch));
	branch->left = branch->right = branch->before = NULL;
	branch->data = data;
	return branch;
}

bool tree_pushBranch(Branch *a, Branch *b, Direction direction) {
	
	if ((a == NULL) || (b == NULL))
		return false;

	b->before = a;

	if (direction == _right) {

		if (a->right != NULL)
			return false;
		a->right = b;
		return true;
	}
	else {
		if (a->left != NULL)
			return false;
		a->left = b;
		return true;
	}
	return false;
}

Branch *tree_searchBranch(Branch *root, void *data, bool(*callback)(void*,void*)) {
	if ((root == NULL) || (callback == NULL) || (data == NULL))
		return NULL;

	if (callback(root->data, data)) {
		return root;
	}

	Branch* branch;

	branch = NULL;

	branch = tree_searchBranch(root->left,data, callback);

	if (branch != NULL)
		return branch;

	branch = tree_searchBranch(root->right, data, callback);

	if (branch != NULL)
		return branch;

	return NULL;
}

void* tree_getData(Branch *b) {
	if (b == NULL)
		return NULL;
	return b->data;
}

void tree_setData(Branch *b, void *data) {
	if ((b == NULL) || (data == NULL))
		return;
	b->data = data;
}

Branch *tree_walkTree(Branch *b, Direction d) {
	if (b == NULL) {
		return NULL;
	}
	
	if (d == _right) {
		return b->right;
	}
	else {
		return b->left;
	}
}

//Verifica se o caminho está correto
void checkWay(bitmap bm, Branch *root, Branch *bTest) {
	int i, length = bitmapGetLength(bm);
	unsigned char bit;
	for (i = 0; i < length; i++) {
		bit = bitmapGetBit(bm, i);
		if (bit == 0) {
			root = tree_walkTree(root, _left);
		}
		else {
			root = tree_walkTree(root, _right);
		}
	}
	if (!(root == bTest)) {
		printf("Tree_getway falhou");
		exit(EXIT_FAILURE);
	}
}

bitmap tree_getWay(Branch *b) {
	if (b == NULL)
		return;
	//Copia do Nó atual para testar o caminho
	Branch *bTest = b;
	bitmap bm;
	bm = bitmapInit(256);
	//Descobre o caminho do Nó até a raiz
	while (b->before != NULL) {
		if (b->before->left == b) {
			bitmapAppendLeastSignificantBit(&bm, 0);
		}
		else {
			bitmapAppendLeastSignificantBit(&bm, 1);
		}
		b = b->before;
	}

	//Inverte o caminho para obter o caminho da raiz ate o Nó
	int i;
	int length;
	bitmap bm2;
	length = bitmapGetLength(bm);
	bm2 = bitmapInit(length);
	for (i = 1; i <= length; i++) {
		bitmapAppendLeastSignificantBit(&bm2, bitmapGetBit(bm, length - i));
	}
	free(bm.contents);
	checkWay(bm2, b, bTest);

	return bm2;
}

bool isLeaf(Branch *auxBranch) {
	if (auxBranch == NULL)
		return false;
	if ((auxBranch->left == NULL) && (auxBranch->right == NULL)) {
		return true;
	}
	return false;
}

void tree_free(Branch *root, void(*callback)(void*)) {
	if ((root == NULL) || (callback == NULL))
		return;
	if (root->left != NULL) {
		tree_free(root->left, callback);
	}
	if (root->right != NULL) {
		tree_free(root->right, callback);
	}
	if (root->data != NULL) {
		callback(root->data);
	}
	free(root);
}