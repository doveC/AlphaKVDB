#include <functional>
#include <iostream>
#include <random>
#include <vector>

#define MAX_LEVEL 8
#define INDEX_PROBABILITY 0.5

namespace Util
{

template<typename K, typename V> 
class Node 
{
public:
    Node() = default;
    Node(K k, V v, int level); 

    K getKey() const;
    V getValue() const;
    void setValue(const V& value);

    std::vector<Node<K, V>*> forward;

    int nodeLevel;

private:
    K key;
    V value;
};

template<typename K, typename V>
Node<K, V>::Node(K k, V v, int level) : nodeLevel(level), key(k), value(v)
{
    // including level 0, so level+1 totally
    forward = std::vector<Node<K, V>*>(level + 1, nullptr);
}

template<typename K, typename V>
K Node<K, V>::getKey() const
{
    return key;
}

template<typename K, typename V>
V Node<K, V>::getValue() const
{
    return value;
}

template<typename K, typename V>
void Node<K, V>::setValue(const V& value)
{
    this->value = value;
}

template<typename K, typename V, typename Comp = std::less<K>>
class SkipList
{
public:
    SkipList(int maxLevel);
    ~SkipList();

    Node<K, V>* searchElement(const K &key) const;
    void insertElement(const K &key, const V &value);
    void deleteElement(const K &key);

    void displayList() const;

    void clear(Node<K, V>* node);

    int randomLevel();
    float randomFloat();

private:
    int currentHighestLevel;
    int maxLevel;

    int currentNodeCount;

    // pointer to sentinel node
    Node<K, V>* header;

    Comp comp;
};

template<typename K, typename V, typename Comp>
SkipList<K, V, Comp>::SkipList(int maxLevel_) : currentHighestLevel(0), maxLevel(maxLevel_), currentNodeCount(0)
{
    header = new Node<K, V>(K{}, V{}, maxLevel);
}

template<typename K, typename V, typename Comp>
SkipList<K, V, Comp>::~SkipList()
{
    if (header->forward[0] != nullptr)
    {
        clear(header->forward[0]);
    }

    delete header;
}

template<typename K, typename V, typename Comp>
void SkipList<K, V, Comp>::clear(Node<K, V>* node)
{
    if (!node)
    {
        return;
    }
    
    if (node->forward[0] != nullptr)
    {
        clear(node->forward[0]);
    }

    delete node;
}

template<typename K, typename V, typename Comp>
Node<K, V>* SkipList<K, V, Comp>::searchElement(const K& key) const
{
    Node<K, V>* current = header;

    for (int currentLevel = currentHighestLevel; currentLevel >= 0; currentLevel--)
    {
        while (current->forward[currentLevel] && comp(current->forward[currentLevel]->getKey(), key))
        {
            current = current->forward[currentLevel];
        }
    }

    current = current->forward[0];

    return (current != nullptr && current->getKey() == key) ? current : nullptr;
}

template<typename K, typename V, typename Comp>
void SkipList<K, V, Comp>::insertElement(const K& key, const V& value)
{
    std::cout << "Inserting key: " << key << std::endl;

    Node<K, V>* current = header;
    std::vector<Node<K, V>*> update(currentHighestLevel + 1, nullptr);

    for (int currentLevel = currentHighestLevel; currentLevel >= 0; currentLevel--)
    {
        while (current->forward[currentLevel] && comp(current->forward[currentLevel]->getKey(), key))
        {
            current = current->forward[currentLevel];
        }

        update[currentLevel] = current;
    }

    current = current->forward[0];

    if (current && current->getKey() == key)
    {
        std::cout << "key: " << key << " already exists." << std::endl;
        return;
    }

    // insert this new node
    int newLevel = randomLevel();
    std::cout << "Random level for new node: " << newLevel << std::endl;
    if (newLevel > currentHighestLevel)
    {
        update.insert(update.end(), newLevel - currentHighestLevel, header);
        currentHighestLevel = newLevel;
    }

    Node<K, V>* newNode = new Node(key, value, newLevel);

    for (int i = 0; i <= newLevel; i++)
    {
        newNode->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = newNode;
    }

    currentNodeCount++;
}

template<typename K, typename V, typename Comp>
void SkipList<K, V, Comp>::deleteElement(const K& key)
{
    std::cout << "Deleting key: " << key << std::endl;

    Node<K, V>* current = header;
    std::vector<Node<K, V>*> update(currentHighestLevel + 1, nullptr);

    for (int currentLevel = currentHighestLevel; currentLevel >= 0; currentLevel--)
    {
        while (current->forward[currentLevel] && comp(current->forward[currentLevel]->getKey(), key))
        {
            current = current->forward[currentLevel];
        }

        update[currentLevel] = current;
    }

    current = current->forward[0];

    if (!current || current->getKey() != key)
    {
        std::cout << "key: " << key << " doesn't exists." << std::endl;
        return;
    }

    for (int i = 0; i <= current->nodeLevel; i++)
    {
        update[i]->forward[i] = current->forward[i];
    }

    delete current;

    // remove empty level
    while (currentHighestLevel > 0 && header->forward[currentHighestLevel] == nullptr)
    {
        currentHighestLevel--;
    }

    currentNodeCount--;

}

template<typename K, typename V, typename Comp>
void SkipList<K, V, Comp>::displayList() const
{
    std::cout << "*****Skip List*****" << std::endl; 

    for (int i = currentHighestLevel; i >= 0; i--) 
    {
        Node<K, V> *node = header->forward[i]; 
        std::cout << "Level " << i << ": ";
        while (node != nullptr) 
        {
            std::cout << "{" << node->getKey() << "-" << node->getValue() << "} ";
            node = node->forward[i];
        }
        std::cout << std::endl;
    }

    std::cout << "*******************" << std::endl; 
}

// return the level of current insert. 0 means no index will be made, 1 means index level 1...
template<typename K, typename V, typename Comp>
int SkipList<K, V, Comp>::randomLevel()
{
    int level = 0;

    while (randomFloat() < INDEX_PROBABILITY && level < maxLevel)
    {
        level++;
    }

    return level;
}

// return a random float number between 0 and 1
template<typename K, typename V, typename Comp>
float SkipList<K, V, Comp>::randomFloat()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distribution(0.0, 1.0);

    float randomfloat = distribution(gen);

    return randomfloat;
}
}