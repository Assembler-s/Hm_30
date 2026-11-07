#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

// level 1
template <typename T>
class Node
{
public:
	T data;
	Node* next;

	Node(const T& value) : data(value), next(nullptr) {}
};

template <typename T>
class Deck
{
private:
	Node<T>* top;
	int count;

public:
	Deck(const Deck& other) = delete;
	Deck& operator = (const Deck& rValue) = delete;

	Deck() : top(nullptr), count(0) {}

	bool isEmpty() const
	{
		return count == 0;
	}

	int size() const
	{
		return count;
	}

	void putOnTop(const T& card)
	{
		Node<T>* new_node = new Node<T>(card);

		new_node->next = top;
		top = new_node;

		count++;
	}

	bool draw(T& card)
	{
		if (count > 0)
		{
			card = top->data;

			Node<T>* next_node = top->next;
			delete top;
			top = next_node;

			count--;

			return true;
		}

		return false;
	}

	void show() const
	{
		Node<T>* ptr = top;

		while (ptr != nullptr)
		{
			cout << ptr->data << " ";
			ptr = ptr->next;
		}
		cout << endl;
	}

	~Deck()
	{
		while (top != nullptr)
		{
			Node<T>* next_top = top->next;
			delete top;
			count--;
			top = next_top;
		}
	}
};
////////////////////////

// level 2
class Player
{
private:
	string name;
	int hp;

public:
	Player(const string& name, int hp) : name(name), hp(hp) {}

	string getName() const
	{
		return name;
	}

	int getHp() const
	{
		return hp;
	}

	void takeDamage(int damage)
	{
		if (hp - damage >= 0)
			hp -= damage;
		else
			hp = 0;
	}

	bool isAlive() const
	{
		return hp > 0;
	}
};

ostream& operator << (ostream& out, const Player& other)
{
	out << other.getName() << " (HP: " << other.getHp() << ")";
	return out;
}

template <typename T>
class TurnQueue
{
private:
	Node<T>* head;
	Node<T>* tail;
	int count;

public:
	TurnQueue(const TurnQueue& other) = delete;
	TurnQueue& operator = (const TurnQueue& rValue) = delete;

	TurnQueue() : head(nullptr), tail(nullptr), count(0) {}

	bool isEmpty() const
	{
		return count == 0;
	}

	int size() const
	{
		return count;
	}

	void enqueue(const T& value)
	{
		Node<T>* new_node = new Node<T>(value);

		if (tail == nullptr)
		{
			head = new_node;
			tail = new_node;
		}
		else
		{
			tail->next = new_node;
			tail = new_node;
		}

		count++;
	}

	T& current()
	{
		return head->data;
	}

	void nextTurn()
	{
		if (count < 2)
			return;

		Node<T>* first = head;

		head = head->next;
		first->next = nullptr;
		tail->next = first;
		tail = first;
	}

	bool removeCurrent()
	{
		if (head == nullptr)
			return false;

		Node<T>* next_node = head->next;
		delete head;
		head = next_node;

		if (head == nullptr)
			tail = nullptr;

		count--;

		return true;
	}

	void show() const
	{
		Node<T>* ptr = head;

		while (ptr != nullptr)
		{
			cout << ptr->data << endl;
			ptr = ptr->next;
		}
	}

	~TurnQueue()
	{
		while (head != nullptr)
		{
			Node<T>* next_node = head->next;
			delete head;
			head = next_node;
		}
	}
};
///////////////////////////////

// level 3
template <typename T>
class GameLog
{
private:
	Node<T>* head;
	Node<T>* tail;

public:
	GameLog(const GameLog& other) = delete;
	GameLog& operator = (const GameLog& rValue) = delete;

	GameLog() : head(nullptr), tail(nullptr) {}

	void addToTail(const T& value)
	{
		Node<T>* new_node = new Node<T>(value);

		if (tail == nullptr)
		{
			head = new_node;
			tail = new_node;
		}
		else
		{
			tail->next = new_node;
			tail = new_node;
		}
	}

	void show() const
	{
		Node<T>* ptr = head;

		while (ptr != nullptr)
		{
			cout << ptr->data << endl;
			ptr = ptr->next;
		}
	}

	void reverse()
	{
		Node<T>* prev = nullptr;
		Node<T>* cur = head;

		tail = head;

		while (cur != nullptr)
		{
			Node<T>* next_node = cur->next;
			cur->next = prev;
			prev = cur;
			cur = next_node;
		}

		head = prev;
	}

	~GameLog()
	{
		while (head != nullptr)
		{
			Node<T>* next_node = head->next;
			delete head;
			head = next_node;
		}
	}
};

int main()
{
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);

	Deck<int> deck;
	int cards[] = { 4, 2, 6, 3, 5, 1, 7, 2, 4, 3 };

	for (int i = 0; i < 10; i++)
	{
		deck.putOnTop(cards[i]);
	}

	deck.show();
	cout << deck.size() << endl;

	int card1 = -1, card2 = -1;

	deck.draw(card1);
	cout << card1 << " " << deck.size() << endl;
	deck.draw(card2);
	cout << card2 << " " << deck.size() << endl;

	Deck<int> empty;
	int card = -1;

	if (!empty.draw(card))
	{
		cout << "Колода пустая\n";
	}
	//////////////////////////////

	// Level 2
	GameLog<string> gameLog;

	TurnQueue<Player> players;

	players.enqueue(Player("Олена", 10));
	players.enqueue(Player("Марко", 10));
	players.enqueue(Player("Тарас", 10));

	while (players.size() > 1 && !deck.isEmpty())
	{
		Player& player = players.current();

		int damage = 0;
		deck.draw(damage);

		player.takeDamage(damage);
		cout << player.getName() << " Тянет карту " << damage << " -> " << player << endl;

		gameLog.addToTail(player.getName() + ": -" + to_string(damage) + " HP");

		if (!player.isAlive())
		{
			cout << player.getName() << " Вылетел из игры\n";
			players.removeCurrent();
		}
		else
		{
			players.nextTurn();
		}
	}

	if (players.size() == 1)
	{
		cout << "Победитель: " << players.current() << endl;
	}
	else
	{
		cout << "Колода закончилась. Остались:\n";
		players.show();
	}
	///////////////////////

	// Level 3
	gameLog.reverse();

	cout << "\nПоследние события игры:\n";
	gameLog.show();

	return 0;
}

/*1) Первой будет карта 3, потому что карты
добавляются наверх колоды. Последняя добавленная карта извлекается первой. Это принцип LIFO*/

/*2) T& card позволяет записать извлечённую карту непосредственно в переданную переменную.
bool сообщает, удалось ли извлечь карту. Если бы метод возвращал T, то при пустой колоде пришлось бы
возвращать какое-то значение, которое могло бы быть обычной картой.*/

/*3) Компилятор создаст конструктор копирования по умолчанию, который скопирует поля, в том числе адрес top.
Оба объекта будут указывать на одну и ту же цепочку узлов. При завершении программы оба деструктора попытаются
удалить одну и ту же память. Это приведёт к неопределённому поведению*/

/*4) size() возвращает количество элементов без прохода по всем узлам.
count нужно изменять в putOnTop() и draw().*/

/*5) Метод изменяет head, tail и поле next узлов.*/

/*6) tail должен получить nullptr, потому что очередь становится пустой. Если этого не сделать, tail останется
висячим указателем на удалённый объект.*/

/*7) Потому что current->next содержит адрес следующего узла, к которому нужно перейти.*/