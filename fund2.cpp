#include <string>
#include <iostream>
#include <fstream>


struct key {
	std::string first;
	std::string second;
	std::string third;
};

struct list {
	int num;
	list* next;
};

class tree {
private:
	struct point {
		point* parent = nullptr;
		key key;
		int color = 0;		// 0 - Red
		list* numOfString;  // 1 - black
		point* left = nullptr, * right = nullptr;
	};
	point* root;
public:

	void initTree() {
		root = nullptr;
	}

	void find(std::string line) {
		key key;
		key = parse(line);
		find(key, 0);
	}

	void del(std::string& line) {
		key key;
		key = parse(line);
		del(key);
	}
	
	void del(key key) {
		if (root != nullptr) {
			point* todel = find(key, 1);
			int delcolor = todel->color;	
			point* child;
			if (getChildrenCount(todel) < 2) {
				child = childLeftOrRight(todel);
				trans(todel, child);
			}
			else {
				point* max = findMax(todel->left);
				todel->key = max->key;	
				todel->numOfString = max->numOfString;
				todel->color = max->color;
				child = childLeftOrRight(max);
				trans(max, child);
			} 
			if (delcolor == 1) {
				fixAfterDel(child);
			}
		}
	}
	
	point* findMax(point* cur) {
		point* curr = cur;
		while (curr->right != nullptr) {
			curr = curr->right;
		}
		return curr;
	}

	int getChildrenCount(point* cur) {
		int count = 0;
		if (cur->left != nullptr) { count++; }
		if (cur->right != nullptr) { count++; }
		return count;
	}

	point* childLeftOrRight(point* cur){
		return (cur->left != nullptr) ? cur->left : cur->right;
	}

	void trans(point* cur1, point* cur2) {
		if (cur1 == root) {
			root = cur2;
		}
		else if (cur1 == cur1->parent->left) {
			cur1->parent->left = cur2;
		}
		else cur1->parent->right = cur2;
		if ((cur1 != nullptr) and (cur2 != nullptr))
		{
			cur2->parent = cur1->parent;
		}
	}

	void fixAfterDel(point* cur) {
		while ((cur != root) or (cur->color == 1)) {
			point* bro;
			if (cur == cur->parent->left) {
				bro = cur->parent->right;
				if (bro->color == 0) {
					bro->color = 1;
					cur->parent->color = 0;
					leftRotate(cur->parent);
					bro = cur->parent->right;
				}
				if (((bro->left == nullptr) or (bro->left->color == 1)) and ((bro->right == nullptr) or (bro->right->color == 1))) {
					bro->color = 0;
					cur = cur->parent; 
				}
				else {
					if (bro->right->color == 1) {
						bro->left->color = 1;
						bro->color = 0;
						rightRotate(bro);
						bro = cur->parent->right;
					}
					bro->color = cur->parent->color;
					cur->parent->color = 1;
					bro->right->color = 1;
					leftRotate(cur->parent);
					cur = root;
				}
			}
			else {
				bro = cur->parent->left;
				if ((bro != nullptr) or (bro->color == 0)) {
					bro->color = 1;
					cur->parent->color = 0;
					rightRotate(cur->parent);
					bro = cur->parent->left;
				}
				if (bro!= nullptr and ((bro->right == nullptr) or (bro->right->color == 1)) and ((bro->left == nullptr) or (bro->left->color == 1))) {
					rightRotate(cur->parent);
					bro = cur->parent->left;
				}
				else {
					if (bro->left->color == 1) {
						bro->right ->color = 1;
						bro->color = 0;
						leftRotate(bro);
						bro = cur->parent->left;
					}
					bro->color = cur->parent->color;
					cur->parent->color = 1;
					bro->left->color = 1;
					rightRotate(cur->parent);
					cur = root;
				}
			}
			cur->color = 1;
		}
	}

	point* find(key key,int sl) {
		if (root != nullptr) {
			point* cur = root;
			std::string s = "";
			std::string compare = compairKey(key, cur->key);
			while ((cur != nullptr) and (compare != "ravny")) {
				if (compare == "perviy") {
					s += "right;";
					cur = cur->right;
				}
				else {
					s += "left;";
					cur = cur->left;
				}
				compare = compairKey(key, cur->key);
			}
			if (cur == nullptr) {
				if (sl == 1) {
					return nullptr;
				}
				std::cout << "Данного элемента нет в списке";
				return nullptr;
			}
			else {
				if (sl == 1) {
					return cur;
				}
				std::cout << "Данный элемент находится, если идти:" << std::endl;
				if (s == "") {
					std::cout << "Это корень" << std::endl;
				}
				while (s != "") {
					std::cout << s.substr(0, s.find(';')-1) << std::endl;
					s.erase(0, s.find(';') - 1);
				}
				std::cout << "В строчках оригинального файла под номерами:" << std::endl;
				list* tmp = cur->numOfString;
				while (tmp->next != cur->numOfString) {
					std::cout << tmp->num << std::endl;
					tmp = tmp->next;
				}
				std::cout << tmp->num << std::endl;
				return nullptr;
			}
		}
	}

	std::string compairKey(key key1, key key2) {
		if ((key1.first == key2.first) and (stoi(key1.second) == stoi(key2.second)) and (stoi(key1.third) == stoi(key2.third))) {
			return "ravny";
		}
		if ((key1.first > key2.first) or ((key1.first == key2.first) and (stoi(key1.second) > stoi(key2.second))) or
			((key1.first == key2.first) and (stoi(key1.second) == stoi(key2.second)) and (stoi(key1.third) > stoi(key2.third)))) {
			return "perviy";
		}
		else {
			return "vtoroy";
		}
	}

	void leftRotate(point* cur) {
		point* tmp = cur->right;
		cur->right = tmp->left;
		if (tmp->left != nullptr) {
			tmp->left->parent = cur;
		}
		tmp->parent = cur->parent;
		if (cur->parent == nullptr) {
			root = tmp;
			std::cout << "WTF";
		}
		else if (cur == cur->parent->left) {
			cur->parent->left = tmp;
		}
		else { cur->parent->right = tmp; }
		tmp->left = cur;
		cur->parent = tmp;
	}

	void rightRotate(point* cur) {
		point* tmp = cur->left;
		cur->left = tmp->right;
		if (tmp->right != nullptr) {
			tmp->right->parent = cur;
		}
		tmp->parent = cur->parent;
		if (cur->parent == nullptr) {
			root = tmp;
		}
		else if (cur == cur->parent->right) {
			cur->parent->right = tmp;
		}
		else { cur->parent->left = tmp; }
		tmp->right = cur;
		cur->parent = tmp;
	}

	void addToTree(key keys, int num) {
		if (root == nullptr) {
			root = new(point);
			root->key = keys;
			root->numOfString = new(list);
			root->numOfString->num = num;
			root->numOfString->next = root->numOfString;
			root->color = 1;
		}
		else {
			point* cur = root, * parent = nullptr;;
			int insert = 0;
			while ((insert == 0) and (cur != nullptr)) {
				std::string compare = compairKey(keys, cur->key);
				if (compare == "ravny") {
					list* tmp,*now = cur->numOfString;
					while (now->next != cur->numOfString) {
						now = now->next;
					}
					tmp = new(list);
					tmp->num = num;
					tmp->next = cur->numOfString;
					now->next = tmp;
					tmp = nullptr; now = nullptr; insert = 1;
				} 
				else if (compare == "perviy") {
					parent = cur;
					cur = cur->right;
				}
				else {
					parent = cur;
					cur = cur->left;
				}
				if (cur == nullptr) {
					cur = new(point);
					cur->parent = parent;
					if (compare == "perviy") {
						parent->right = cur;
					}
					else {
						parent->left = cur;
					}
					cur->key = keys;
					cur->numOfString = new(list);
					cur->numOfString->num = num;
					cur->numOfString->next = cur->numOfString;
					cur->color = 0;
					insert = 1;
					fixAfterInsert(cur);
				}
			}
		}
	}

	void fixAfterInsert(point* cur) {
		if ((cur != root) and (cur->parent != root)) {
			while ((cur->parent != nullptr) and (cur->parent->color == 0)) {
				if (cur->parent == cur->parent->parent->left) {
					point* uncle = cur->parent->parent->right;
					if ((uncle == nullptr) or (uncle->color == 1)) {
						if (cur == cur->parent->right) {
							cur = cur->parent;
							leftRotate(cur);
						}
						cur->parent->color = 1;
						cur->parent->parent->color = 0;
						rightRotate(cur->parent->parent);
					}
					else {
						cur->parent->color = 1;
						uncle->color = 1;
						cur->parent->parent->color = 0;
						cur = cur->parent->parent;
					}
				}
				else {
					point* uncle = cur->parent->parent->left;
					if ((uncle == nullptr) or (uncle->color == 1)) {
						if (cur == cur->parent->left) {
							cur = cur->parent;
							rightRotate(cur);
						}
						cur->parent->color = 1;
						cur->parent->parent->color = 0;
						leftRotate(cur->parent->parent);
					}
					else {
						cur->parent->color = 1;
						uncle->color = 1;
						cur->parent->parent->color = 0;
						cur = uncle->parent;
					}
				}
			}
			root->color = 1;
		}
	}

	void obxod() {
		std::string line = "";
		if (root != nullptr) {
			obxod(root,line);
		}
		std::ofstream fout("output.txt");
		while (line != "") {
			fout << line.substr(0, line.find(' ')) << std::endl;
			line.erase(0, line.find(' ')+1);
		}
		fout.close();
	}

	void obxod(point* cur, std::string& line) {
		if (cur->left != nullptr) 
		{
			obxod(cur->left,line);
		}
		if (cur->right != nullptr)
		{
			obxod(cur->right,line);
		}
		line = line + cur->key.first + cur->key.second + cur->key.third + ' ';
	}
	
	void printroot() {
		std::cout << root->key.first << root->key.second << root->key.third;
	}

	key parse(std::string& line) {
		key key;
		key.first = line.substr(0, 1);
		line.erase(0, 1);
		if (line.length() == 5) {
			key.second = line.substr(0, 2);
			line.erase(0, 2);
		}
		else {
			key.second = line.substr(0, 1);
			line.erase(0, 1);
		}
		key.third = line.substr(0);
		line.erase(0);
		return key;
	}

	void print() {
		print(root, 0);
	}

	void print(point* cur ,int count) {
		if (cur->right != nullptr) {
			print(cur->right, count + 1);
		}
		for (int i = 0; i < 14 * count; i++) {
			std::cout << ' ';
		}
		std::cout  << cur->key.first << cur->key.second << cur->key.third;
		if (cur->color == 1) {
			std::cout << "(Black)" << std::endl;
		}
		else {
			std::cout << "(Red)" << std::endl;
		}
		if (cur->left != nullptr) {
			print(cur->left, count + 1);
		}
	}

	void readFromFile() {
		std::string line;
		std::ifstream fin("in.txt");
		if (fin.is_open()) {
			int num = 0;
			while (std::getline(fin, line)) {
				num++;
				key key = parse(line);
				addToTree(key, num);
			}
		}
		fin.close();
	}
};

int main()
{
	tree Tree;
	setlocale(LC_ALL, "Russian");
	Tree.initTree();
	Tree.readFromFile();
	Tree.print();
	std::cout << "Что удалять?" << std::endl;
	std::string line;
	std::cin >> line;
	Tree.del(line);
	Tree.print();
	Tree.obxod();
}
