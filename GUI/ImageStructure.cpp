#include "ImageStructure.h"

void LinkedList::append(QGraphicsPixmapItem* item)
{
	if (size >= maxSize) {
		std::cout << "链表已满，无法添加新节点！" << std::endl;
		return;
	}

	Node* newNode = new Node(item);

	if (head == nullptr) {
		head = newNode;
	}
	else {
		Node* current = head;
		while (current->next != nullptr) {
			current = current->next;
		}
		current->next = newNode;
	}
	size++;
}

void LinkedList::remove(Node* node)
{
	if (head == nullptr) {
		std::cout << "链表为空，无法删除节点！" << std::endl;
		return;
	}

	if (node == head) {
		head = head->next;
		delete node->data;
		delete node;
		size--;
		return;
	}

	Node* current = head;
	while (current->next != nullptr && current->next != node) {
		current = current->next;
	}

	if (current->next == nullptr) {
		std::cout << "未找到指定节点！" << std::endl;
		return;
	}

	current->next = node->next;
	delete node->data;
	delete node;
	size--;
}

QGraphicsPixmapItem* LinkedList::getNodeData(int index)
{
	if (index < 0 || index >= size) {
		std::cout << "索引越界！" << std::endl;
		return nullptr;
	}

	Node* current = head;
	for (int i = 0; i < index; i++) {
		current = current->next;
	}

	return current->data;
}

Node* LinkedList::getNodeNumber(int index)
{
	if (index < 0 || index >= size) {
		std::cout << "索引越界！" << std::endl;
		return nullptr;
	}

	Node* current = head;
	for (int i = 0; i < index; i++) {
		current = current->next;
	}

	return current;

}

void LinkedList::clear()
{
	Node* current = head;
	while (current != nullptr) {
		Node* temp = current;
		current = current->next;
		delete temp->data;
		delete temp;
	}
	head = nullptr;
	size = 0;
}

