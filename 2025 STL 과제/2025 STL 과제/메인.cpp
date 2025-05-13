#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <array>
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
		std::cout << "�̸�: " << name
			<< ", ���̵�: " << id
			<< ", ����: " << score
			<< ", �ڿ���: " << num
			<< "\n����� ����: " << p.get() << "\n" << std::endl;

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
std::array<const Player*, 250'0000> sortedById;
std::array<const Player*, 250'0000> sortedByName;
std::array<const Player*, 250'0000> sortedByScore;

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

		std::cout << "���� ������ Player�� ����\n";
		players.back().show();
	}
	
	/*2. ������ ���� ū Player�� ã�� ȭ�鿡 ����϶�.*/
	{
		std::cout << "������ ���� ū Player�� ã�� ȭ�鿡 ���\n";
		(*std::max_element(players.begin(), players.end(), [](const Player& p1, const Player& p2) {
			return p1.getScore() < p2.getScore();
		})).show();

		/*Player�� ��� ������ ����Ͽ� ȭ�鿡 ����϶�.*/
		std::cout << "��� ����: " << std::accumulate(players.begin(), players.end(), 0LL,
			[](long long sum, const Player& p) {
			return sum + p.getScore();
		}) / players.size() << std::endl;
	}

	/*3. id�� ���� ���� ��ü�� ã�� "�������̵�.txt"�� ����϶�.
		id�� ���� ��ü�� ��� �� ������ ȭ�鿡 ����϶�.
		���Ͽ��� id�� ���� Player ��ü�� �̸��� ���̵� �� �� �� ����Ѵ�.*/
	/*std::unordered_map<size_t, std::vector<const Player*>> idMap;

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
	std::cout << "\nid�� �ߺ��� Player�� �� " << count << "���Դϴ�.\n";*/

	/*4. Player�� ��� p�� ����Ű�� �޸𸮿��� ���Ͽ��� ���� num���� char��
		����Ǿ� �־�� �Ѵ�
		�޸𸮿� ����� char�� ������������ �����϶�.
		'a'�� 10���� �̻��� Player�� ������ ȭ�鿡 ����϶�.*/
	{
		//int a10PlayerCount{};

		//for (Player& p : players) {
		//	std::sort(p.getP(), p.getP() + p.getNum());

		//	// a ���� ����
		//	int aCount = std::count(p.getP(), p.getP() + p.getNum(), 'a');
		//	if (aCount >= 10) {
		//		++a10PlayerCount;
		//	}
		//}

		//std::cout << "\n'a'�� 10�� �̻��� Player�� " << a10PlayerCount << "���Դϴ�.\n";
	}


	// 5. [ LOOP ] id�� �Է¹޾� �����ϴ� id��� ���� ������ �� ���� ȭ�� ����϶�.
	for (size_t i = 0; i < players.size(); ++i) {
		sortedById[i] = &players[i];
		sortedByName[i] = &players[i];
		sortedByScore[i] = &players[i];
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
		std::cout << "\nȮ���� id�� �Է��ϼ��� (����� 0): ";
		size_t inputId;
		std::cin >> inputId;

		if (inputId == 0) break;

		/*Player�� id ���� ������������ �����Ͽ��� ��
		�ش� id ���� �հ� �� Player�� ������ ����Ѵ�.
		id�� ���� Player�� �� �̻��̸� �̵��� ������ ��� ����Ͽ��� �Ѵ�.*/
		std::cout << "Player�� id ���� ������������ �����Ͽ��� ��\n";

		auto lower = std::lower_bound(sortedById.begin(), sortedById.end(), inputId,
			[](const Player* p, size_t id) {
			return p->getId() < id;
		});

		auto upper = std::upper_bound(sortedById.begin(), sortedById.end(), inputId,
			[](size_t id, const Player* p) {
			return id < p->getId();
		});

		if (lower == upper) {
			std::cout << "�ش� ID�� ���� Player�� �����ϴ�.\n";
			continue;
		}

		std::cout << "\n[ID ���� ���� �� ���� ID Player��]\n";
		for (auto it = lower; it != upper; ++it) {
			(*it)->show();  // �� Player ��ü�� ������ ���
		}

		// �� Player
		if (lower != sortedById.begin()) {
			std::cout << "\n[�� Player]\n";
			(*(lower - 1))->show();  // lower�� �ٷ� �� Player ���� ���
		}

		// �� Player
		if (upper != sortedById.end()) {
			std::cout << "\n[�� Player]\n";
			(*upper)->show();  // upper�� ����Ű�� Player ���� ���
		}

		/*Player�� name ���� ������������ �����Ͽ��� ��
		�ش� name ���� �հ� �� Player�� ������ ����Ѵ�.
		���� name�� ������ ��� �ٷ� �յ� �Ѹ��� Player ������ ����ϸ� �ȴ�.*/
		std::cout << "Player�� name ���� ������������ �����Ͽ��� ��\n";
		
		auto lowerName = std::lower_bound(sortedByName.begin(), sortedByName.end(), inputId,
			[](const Player* p, size_t id) {
			return p->getName() < id;  // �̸� �������� ��
		});

		auto upperName = std::upper_bound(sortedByName.begin(), sortedByName.end(), inputId,
			[](size_t id, const Player* p) {
			return id < p->getName();  // �̸� �������� ��
		});

		if (lowerName == upperName) {
			std::cout << "�ش� �̸��� ���� Player�� �����ϴ�.\n";
		}
		else {
			std::cout << "\n[Name ���� ���� �� ���� Name Player��]\n";
			for (auto it = lowerName; it != upperName; ++it) {
				(*it)->show();  // �� Player ��ü�� ������ ���
			}

			// �� Player
			if (lowerName != sortedByName.begin()) {
				std::cout << "\n[�� Player]\n";
				(*(lowerName - 1))->show();  // lowerName�� �ٷ� �� Player ���� ���
			}

			// �� Player
			if (upperName != sortedByName.end()) {
				std::cout << "\n[�� Player]\n";
				(*upperName)->show();  // upperName�� ����Ű�� Player ���� ���
			}
		}

		/*Player�� score ���� ������������ �����Ͽ��� ��
		�ش� score ���� �հ� �� Player�� ������ ����Ѵ�.
		���� score�� ������ ��� �ٷ� �յ� �Ѹ��� Player ������ ����ϸ� �ȴ�.*/
		std::cout << "Player�� score ���� ������������ �����Ͽ��� ��\n";

		auto lowerScore = std::lower_bound(sortedByScore.begin(), sortedByScore.end(), inputId,
			[](const Player* p, size_t id) {
			return p->getScore() < id;  // ���� �������� ��
		});

		auto upperScore = std::upper_bound(sortedByScore.begin(), sortedByScore.end(), inputId,
			[](size_t id, const Player* p) {
			return id < p->getScore();  // ���� �������� ��
		});

		if (lowerScore == upperScore) {
			std::cout << "�ش� ������ ���� Player�� �����ϴ�.\n";
		}
		else {
			std::cout << "\n[Score ���� ���� �� ���� Score Player��]\n";
			for (auto it = lowerScore; it != upperScore; ++it) {
				(*it)->show();  // �� Player ��ü�� ������ ���
			}

			// �� Player
			if (lowerScore != sortedByScore.begin()) {
				std::cout << "\n[�� Player]\n";
				(*(lowerScore - 1))->show();  // lowerScore�� �ٷ� �� Player ���� ���
			}

			// �� Player
			if (upperScore != sortedByScore.end()) {
				std::cout << "\n[�� Player]\n";
				(*upperScore)->show();  // upperScore�� ����Ű�� Player ���� ���
			}
		}
	}
}