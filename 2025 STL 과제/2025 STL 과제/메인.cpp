#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <array>
#include <print>
#include <ranges>
#include <algorithm>
#include <numeric>
#include <unordered_map>	
#include <vector>

class Player {
private:
	std::string name;			// �̸�, ����[3, 15], ['a', 'z']�θ� ����
	int score;					// ����
	size_t id;					// ���̵�, ��ġ�� ���̵� ���� �� ����
	size_t num;					// free store�� Ȯ���� ����Ʈ ��
	std::unique_ptr<char[]> p;	// free store�� Ȯ���� �޸�

public:
	std::string getName() const {
		return name;
	}

	int getScore() const { 
		return score; 
	}

	size_t getId() const {
		return id;
	}

	size_t getNum() const {
		return num;
	}

	char* getP() const {
		return p.get();
	}

	void show() const {
		std::println("�̸�:{:>15}, ���̵�:{:20}, ����:{:20}, �ڿ���:{:20}\n����� ����:{}\n", name, id, score, num, p.get());
	}

	void read(std::istream& is) {
		is.read(reinterpret_cast<char*>(this), sizeof(Player));
		p.release();
		p = std::make_unique<char[]>(num);
		is.read(p.get(), num);
	}

	bool operator<(const Player& other) const {
		return this->score < other.score;  // ���� �������� ��
	}
};

std::array<Player, 250'0000> players;

int main()
{
	// ���� �б�
	std::ifstream in{ "2025 STL ���� ���� - 2023180007", std::ios::binary };
	if (not in) {
		std::cout << "���� �б� ����" << std::endl;
		return 20250410;
	}

	/*1. ���Ͽ� ������ ��� Player ������ �о� �����̳ʿ� �����϶�.
		���� ������ Player�� ������ ������ ���� �������� ȭ�鿡 ����϶�.*/
	{
		for (Player& player : players) {
			player.read(in);
		}

		std::println("���� ������ Player�� ����");
		players.back().show();
	}
	
	/*2. ������ ���� ū Player�� ã�� ȭ�鿡 ����϶�.*/
	{
		std::println("������ ���� ū Player�� ã�� ȭ�鿡 ���");
		(*std::max_element(players.begin(), players.end())).show();

		/*Player�� ��� ������ ����Ͽ� ȭ�鿡 ����϶�.*/
		std::cout << "��� ����: " << std::accumulate(players.begin(), players.end(), 0LL,
			[](long long sum, const Player& p) {
			return sum + p.getScore();
		}) / players.size() << std::endl;
	}

	/*3. id�� ���� ���� ��ü�� ã�� "�������̵�.txt"�� ����϶�.
		id�� ���� ��ü�� ��� �� ������ ȭ�鿡 ����϶�.
		���Ͽ��� id�� ���� Player ��ü�� �̸��� ���̵� �� �� �� ����Ѵ�.*/
	std::unordered_map<size_t, std::vector<const Player*>> idMap;

	for (const Player& player : players) {
		idMap[player.getId()].push_back(&player);
	}

	std::ofstream out{ "�������̵�.txt" };
	int count = 0;

	for (const auto& [id, players] : idMap) {
		if (players.size() >= 2) {
			for (const auto* player : players) {
				out << player->getId() << " " << player->getName() << "\n";
				++count;
			}
		}
	}
	std::println("\nid�� �ߺ��� Player�� �� {}���Դϴ�.", count);

	/*4. Player�� ��� p�� ����Ű�� �޸𸮿��� ���Ͽ��� ���� num���� char��
		����Ǿ� �־�� �Ѵ�
		�޸𸮿� ����� char�� ������������ �����϶�.
		'a'�� 10���� �̻��� Player�� ������ ȭ�鿡 ����϶�.*/
	{
		int a10PlayerCount{};

		for (Player& player : players) {
			char* p = player.getP();
			size_t num = player.getNum();
			std::sort(p, p + num);

			// a ���� ����
			int aCount = std::count(p, p + num, 'a');
			if (aCount >= 10) {
				++a10PlayerCount;
			}
		}

		std::println("\n'a'�� 10�� �̻��� Player�� {}���Դϴ�.", a10PlayerCount);
	}


	// 5. [ LOOP ] id�� �Է¹޾� �����ϴ� id��� ���� ������ �� ���� ȭ�� ����϶�.
	std::vector<const Player*> sortedById;
	std::vector<const Player*> sortedByName;
	std::vector<const Player*> sortedByScore;

	for (const Player& p : players) {
		sortedById.push_back(&p);
		sortedByName.push_back(&p);
		sortedByScore.push_back(&p);
	}


	std::sort(sortedById.begin(), sortedById.end(), [](const Player* a, const Player* b) {
		return a->getId() < b->getId();
	});

	std::sort(sortedByName.begin(), sortedByName.end(), [](const Player* a, const Player* b) {
		return a->getName() < b->getName();
	});

	std::sort(sortedByScore.begin(), sortedByScore.end(), [](const Player* a, const Player* b) {
		return a->getScore() < b->getScore();
	});


	while (true) {
		std::print("\nȮ���� id�� �Է��ϼ��� (����� 0): ");
		size_t inputId;
		std::cin >> inputId;

		if (inputId == 0) break;

		/*Player�� id ���� ������������ �����Ͽ��� ��
		�ش� id ���� �հ� �� Player�� ������ ����Ѵ�.
		id�� ���� Player�� �� �̻��̸� �̵��� ������ ��� ����Ͽ��� �Ѵ�.*/
		std::println("Player�� id ���� ������������ �����Ͽ��� ��\n");

		auto it = std::ranges::lower_bound(
			sortedById, inputId,
			{},  // projection ����
			[](const Player* p) { return p->getId(); }
		);

		if (idMap[inputId].size() >= 2) {
			for (const auto& id : idMap[inputId]) {
				id->show();
			}
		}
		else if (it != sortedById.end() and (*it)->getId() == inputId) {
			if (it != sortedById.begin()) {
				std::println("\n[���� Player]");
				(*(it - 1))->show();
			}

			std::println("\n[�ش� Player]");
			(*it)->show();

			if ((it + 1) != sortedById.end()) {
				std::println("\n[���� Player]");
				(*(it + 1))->show();
			}
		}
		else {
			std::println("�ش� ID�� ���� Player�� ã�� �� �����ϴ�.");
		}

		/*Player�� name ���� ������������ �����Ͽ��� ��
		�ش� name ���� �հ� �� Player�� ������ ����Ѵ�.
		���� name�� ������ ��� �ٷ� �յ� �Ѹ��� Player ������ ����ϸ� �ȴ�.*/
		std::println("Player�� name ���� ������������ �����Ͽ��� ��\n");
		

		/*Player�� score ���� ������������ �����Ͽ��� ��
		�ش� score ���� �հ� �� Player�� ������ ����Ѵ�.
		���� score�� ������ ��� �ٷ� �յ� �Ѹ��� Player ������ ����ϸ� �ȴ�.*/
		std::println("Player�� score ���� ������������ �����Ͽ��� ��\n");
	}
}