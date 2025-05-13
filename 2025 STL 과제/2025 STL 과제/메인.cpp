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
	std::string name;			// 이름, 길이[3, 15], ['a', 'z']로만 구성
	int score;					// 점수
	size_t id;					// 아이디, 겹치는 아이디 있을 수 있음
	size_t num;					// free store에 확보한 바이트 수
	std::unique_ptr<char[]> p;	// free store에 확보한 메모리

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
		std::cout << "이름: " << name
			<< ", 아이디: " << id
			<< ", 점수: " << score
			<< ", 자원수: " << num
			<< "\n저장된 글자: " << p.get() << "\n" << std::endl;

	}

	void read(std::istream& is) {
		is.read(reinterpret_cast<char*>(this), sizeof(Player));
		p.release();
		p = std::make_unique<char[]>(num);
		is.read(p.get(), num);
	}

	bool operator<(const Player& other) const {
		return this->score < other.score;  // 점수 기준으로 비교
	}
};

std::array<Player, 250'0000> players;
std::array<const Player*, 250'0000> sortedById;
std::array<const Player*, 250'0000> sortedByName;
std::array<const Player*, 250'0000> sortedByScore;

int main()
{
	// 파일 읽기
	std::ifstream in{ "2025 STL 과제 파일 - 2023180007", std::ios::binary };
	if (not in) {
		std::cout << "파일 읽기 실패" << std::endl;
		return 20250410;
	}

	/*1. 파일에 저장한 모든 Player 정보를 읽어 컨테이너에 저장하라.
		제일 마지막 Player의 정보를 다음과 같은 형식으로 화면에 출력하라.*/
	{
		for (Player& player : players) {
			player.read(in);
		}

		std::cout << "제일 마지막 Player의 정보\n";
		players.back().show();
	}
	
	/*2. 점수가 가장 큰 Player를 찾아 화면에 출력하라.*/
	{
		std::cout << "점수가 가장 큰 Player를 찾아 화면에 출력\n";
		(*std::max_element(players.begin(), players.end(), [](const Player& p1, const Player& p2) {
			return p1.getScore() < p2.getScore();
		})).show();

		/*Player의 평균 점수를 계산하여 화면에 출력하라.*/
		std::cout << "평균 점수: " << std::accumulate(players.begin(), players.end(), 0LL,
			[](long long sum, const Player& p) {
			return sum + p.getScore();
		}) / players.size() << std::endl;
	}

	/*3. id가 서로 같은 객체를 찾아 "같은아이디.txt"에 기록하라.
		id가 같은 객체는 모두 몇 개인지 화면에 출력하라.
		파일에는 id가 같은 Player 객체의 이름과 아이디를 한 줄 씩 기록한다.*/
	/*std::unordered_map<size_t, std::vector<const Player*>> idMap;

	for (const Player& player : players) {
		idMap[player.getId()].push_back(&player);
	}

	std::ofstream out{ "같은아이디.txt" };
	int count = 0;

	for (const auto& [id, players] : idMap) {
		if (players.size() >= 2) {
			for (const auto* player : players) {
				out << player->getId() << " " << player->getName() << "\n";
				++count;
			}
		}
	}
	std::cout << "\nid가 중복된 Player는 총 " << count << "명입니다.\n";*/

	/*4. Player의 멤버 p가 가리키는 메모리에는 파일에서 읽은 num개의 char가
		저장되어 있어야 한다
		메모리에 저장된 char를 오름차순으로 정렬하라.
		'a'가 10글자 이상인 Player의 개수를 화면에 출력하라.*/
	{
		//int a10PlayerCount{};

		//for (Player& p : players) {
		//	std::sort(p.getP(), p.getP() + p.getNum());

		//	// a 개수 세기
		//	int aCount = std::count(p.getP(), p.getP() + p.getNum(), 'a');
		//	if (aCount >= 10) {
		//		++a10PlayerCount;
		//	}
		//}

		//std::cout << "\n'a'가 10개 이상인 Player는 " << a10PlayerCount << "명입니다.\n";
	}


	// 5. [ LOOP ] id를 입력받아 존재하는 id라면 다음 내용을 한 번에 화면 출력하라.
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
		std::cout << "\n확인할 id를 입력하세요 (종료는 0): ";
		size_t inputId;
		std::cin >> inputId;

		if (inputId == 0) break;

		/*Player를 id 기준 오름차순으로 정렬하였을 때
		해당 id 포함 앞과 뒤 Player의 정보를 출력한다.
		id가 같은 Player가 둘 이상이면 이들의 정보를 모두 출력하여야 한다.*/
		std::cout << "Player를 id 기준 오름차순으로 정렬하였을 때\n";

		auto lower = std::lower_bound(sortedById.begin(), sortedById.end(), inputId,
			[](const Player* p, size_t id) {
			return p->getId() < id;
		});

		auto upper = std::upper_bound(sortedById.begin(), sortedById.end(), inputId,
			[](size_t id, const Player* p) {
			return id < p->getId();
		});

		if (lower == upper) {
			std::cout << "해당 ID를 가진 Player가 없습니다.\n";
			continue;
		}

		std::cout << "\n[ID 기준 정렬 시 같은 ID Player들]\n";
		for (auto it = lower; it != upper; ++it) {
			(*it)->show();  // 각 Player 객체의 정보를 출력
		}

		// 앞 Player
		if (lower != sortedById.begin()) {
			std::cout << "\n[앞 Player]\n";
			(*(lower - 1))->show();  // lower의 바로 앞 Player 정보 출력
		}

		// 뒤 Player
		if (upper != sortedById.end()) {
			std::cout << "\n[뒤 Player]\n";
			(*upper)->show();  // upper가 가리키는 Player 정보 출력
		}

		/*Player를 name 기준 오름차순으로 정렬하였을 때
		해당 name 포함 앞과 뒤 Player의 정보를 출력한다.
		같은 name이 여럿일 경우 바로 앞뒤 한명의 Player 정보만 출력하면 된다.*/
		std::cout << "Player를 name 기준 오름차순으로 정렬하였을 때\n";
		
		auto lowerName = std::lower_bound(sortedByName.begin(), sortedByName.end(), inputId,
			[](const Player* p, size_t id) {
			return p->getName() < id;  // 이름 기준으로 비교
		});

		auto upperName = std::upper_bound(sortedByName.begin(), sortedByName.end(), inputId,
			[](size_t id, const Player* p) {
			return id < p->getName();  // 이름 기준으로 비교
		});

		if (lowerName == upperName) {
			std::cout << "해당 이름을 가진 Player가 없습니다.\n";
		}
		else {
			std::cout << "\n[Name 기준 정렬 시 같은 Name Player들]\n";
			for (auto it = lowerName; it != upperName; ++it) {
				(*it)->show();  // 각 Player 객체의 정보를 출력
			}

			// 앞 Player
			if (lowerName != sortedByName.begin()) {
				std::cout << "\n[앞 Player]\n";
				(*(lowerName - 1))->show();  // lowerName의 바로 앞 Player 정보 출력
			}

			// 뒤 Player
			if (upperName != sortedByName.end()) {
				std::cout << "\n[뒤 Player]\n";
				(*upperName)->show();  // upperName이 가리키는 Player 정보 출력
			}
		}

		/*Player를 score 기준 오름차순으로 정렬하였을 때
		해당 score 포함 앞과 뒤 Player의 정보를 출력한다.
		같은 score가 여럿일 경우 바로 앞뒤 한명의 Player 정보만 출력하면 된다.*/
		std::cout << "Player를 score 기준 오름차순으로 정렬하였을 때\n";

		auto lowerScore = std::lower_bound(sortedByScore.begin(), sortedByScore.end(), inputId,
			[](const Player* p, size_t id) {
			return p->getScore() < id;  // 점수 기준으로 비교
		});

		auto upperScore = std::upper_bound(sortedByScore.begin(), sortedByScore.end(), inputId,
			[](size_t id, const Player* p) {
			return id < p->getScore();  // 점수 기준으로 비교
		});

		if (lowerScore == upperScore) {
			std::cout << "해당 점수를 가진 Player가 없습니다.\n";
		}
		else {
			std::cout << "\n[Score 기준 정렬 시 같은 Score Player들]\n";
			for (auto it = lowerScore; it != upperScore; ++it) {
				(*it)->show();  // 각 Player 객체의 정보를 출력
			}

			// 앞 Player
			if (lowerScore != sortedByScore.begin()) {
				std::cout << "\n[앞 Player]\n";
				(*(lowerScore - 1))->show();  // lowerScore의 바로 앞 Player 정보 출력
			}

			// 뒤 Player
			if (upperScore != sortedByScore.end()) {
				std::cout << "\n[뒤 Player]\n";
				(*upperScore)->show();  // upperScore가 가리키는 Player 정보 출력
			}
		}
	}
}