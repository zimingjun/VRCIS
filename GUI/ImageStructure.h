#pragma once
#include <iostream>
#include <QGraphicsPixmapItem>

class Node {
public:
    QGraphicsPixmapItem* data;
    Node* next;

    Node(QGraphicsPixmapItem* item) : data(item), next(nullptr) {}
};

class LinkedList {
private:
    Node* head;
    int size;
    int maxSize;

public:
    LinkedList(int max) : head(nullptr), size(0), maxSize(max) {}

    void append(QGraphicsPixmapItem* item);

    void remove(Node* node);

    QGraphicsPixmapItem* getNodeData(int index);

    Node* getNodeNumber(int index);

    void clear();
};
