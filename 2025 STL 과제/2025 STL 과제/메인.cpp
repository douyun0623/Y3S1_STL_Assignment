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
		std::println("이름:{:>15}, 아이디:{:20}, 점수:{:20}, 자원수:{:20}\n저장된 글자:{}\n", name, id, score, num, p.get());
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

		std::println("제일 마지막 Player의 정보");
		players.back().show();
	}
	
	/*2. 점수가 가장 큰 Player를 찾아 화면에 출력하라.*/
	{
		std::println("점수가 가장 큰 Player를 찾아 화면에 출력");
		(*std::max_element(players.begin(), players.end())).show();

		/*Player의 평균 점수를 계산하여 화면에 출력하라.*/
		std::cout << "평균 점수: " << std::accumulate(players.begin(), players.end(), 0LL,
			[](long long sum, const Player& p) {
			return sum + p.getScore();
		}) / players.size() << std::endl;
	}

	/*3. id가 서로 같은 객체를 찾아 "같은아이디.txt"에 기록하라.
		id가 같은 객체는 모두 몇 개인지 화면에 출력하라.
		파일에는 id가 같은 Player 객체의 이름과 아이디를 한 줄 씩 기록한다.*/
	std::unordered_map<size_t, std::vector<const Player*>> idMap;

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
	std::println("\nid가 중복된 Player는 총 {}명입니다.", count);

	/*4. Player의 멤버 p가 가리키는 메모리에는 파일에서 읽은 num개의 char가
		저장되어 있어야 한다
		메모리에 저장된 char를 오름차순으로 정렬하라.
		'a'가 10글자 이상인 Player의 개수를 화면에 출력하라.*/
	{
		int a10PlayerCount{};

		for (Player& player : players) {
			char* p = player.getP();
			size_t num = player.getNum();
			std::sort(p, p + num);

			// a 개수 세기
			int aCount = std::count(p, p + num, 'a');
			if (aCount >= 10) {
				++a10PlayerCount;
			}
		}

		std::println("\n'a'가 10개 이상인 Player는 {}명입니다.", a10PlayerCount);
	}


	// 5. [ LOOP ] id를 입력받아 존재하는 id라면 다음 내용을 한 번에 화면 출력하라.
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
		std::print("\n확인할 id를 입력하세요 (종료는 0): ");
		size_t inputId;
		std::cin >> inputId;

		if (inputId == 0) break;

		/*Player를 id 기준 오름차순으로 정렬하였을 때
		해당 id 포함 앞과 뒤 Player의 정보를 출력한다.
		id가 같은 Player가 둘 이상이면 이들의 정보를 모두 출력하여야 한다.*/
		std::println("Player를 id 기준 오름차순으로 정렬하였을 때\n");

		auto it = std::ranges::lower_bound(
			sortedById, inputId,
			{},  // projection 생략
			[](const Player* p) { return p->getId(); }
		);

		if (idMap[inputId].size() >= 2) {
			for (const auto& id : idMap[inputId]) {
				id->show();
			}
		}
		else if (it != sortedById.end() and (*it)->getId() == inputId) {
			if (it != sortedById.begin()) {
				std::println("\n[앞의 Player]");
				(*(it - 1))->show();
			}

			std::println("\n[해당 Player]");
			(*it)->show();

			if ((it + 1) != sortedById.end()) {
				std::println("\n[뒤의 Player]");
				(*(it + 1))->show();
			}
		}
		else {
			std::println("해당 ID를 가진 Player를 찾을 수 없습니다.");
		}

		/*Player를 name 기준 오름차순으로 정렬하였을 때
		해당 name 포함 앞과 뒤 Player의 정보를 출력한다.
		같은 name이 여럿일 경우 바로 앞뒤 한명의 Player 정보만 출력하면 된다.*/
		std::println("Player를 name 기준 오름차순으로 정렬하였을 때\n");
		

		/*Player를 score 기준 오름차순으로 정렬하였을 때
		해당 score 포함 앞과 뒤 Player의 정보를 출력한다.
		같은 score가 여럿일 경우 바로 앞뒤 한명의 Player 정보만 출력하면 된다.*/
		std::println("Player를 score 기준 오름차순으로 정렬하였을 때\n");
	}
}