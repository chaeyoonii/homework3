/*
 * Problem 5: refactoring
 *
 * 학번:202502190
 * 이름:김채윤
 * ─────────────────────────────────────────────────────────────
 * 프로그램의 동작을 바꾸지 않고 코드를 리팩터링하세요.
 *
 * 다음 내용을 중심으로 개선해 주세요.
 * - Meaningful names
 * - Small functions
 * - Do one thing
 * - One level of abstraction
 * - Reduce duplicated code
 * - Reduce deeply nested conditionals
 * - Appropriate use of reference / const reference
 * - Consistent formatting
 *
 * 요구 사항
 * 1. 프로그램의 출력 결과와 동작을 유지해 주세요.
 * 2. p, calc, f, run, r, n과 같이 의미가 불분명한 이름을 개선해 주세요.
 * 3. 하나의 함수에서 여러 역할을 수행하고 있다면 적절히 분리해 주세요.
 * 4. 중복되는 quality 범위 처리 코드를 줄여 주세요.
 * 5. 중첩된 if 문을 읽기 쉬운 형태로 개선해 주세요.
 * 6. 값을 변경하지 않는 함수 parameter에는 가능한 경우 const를 적용해 주세요.
 * 7. 불필요한 복사가 발생하지 않도록 reference를 적절히 사용해 주세요.
 * 8. 아직 배우지 않은 class, inheritance, template, design pattern은 사용하지 마세요.
 *
 * 반드시 유지할 동작
 * 리팩터링 전후에 다음 동작이 동일해야 합니다.
 * - 세 날짜 동안의 item 상태 변화
 * - Total price 계산
 * - High-quality item 개수 계산
 * - Expired item 개수 계산
 * - Cheese 검색
 * - 검색된 Cheese 가격 500 감소
 *
 * ─────────────────────────────────────────────────────────────
 */
/*
 * 과제
 * 1. 가장 먼저 개선하고 싶었던 부분은 무엇이었나요?
 * >의미가 불분명한 함수 이름과 하나의 함수에서 여러 역할을 수행하는 동작들을 분리해 각 함수의 역할을 더 강화시키고 개선하고 싶었습니다.
 * 
 * 2. 함수 하나를 새로 추출했다면, 그 함수가 담당하는 한 가지 역할은 무엇인가요?
 * >calculateTotalPrice()는 모든 아이템의 가격을 합산하는 역할만 담당하도록 했습니다.또한 
 * 3. Item&와 const Item& 중 어떤 것을 사용했으며 그 이유는 무엇인가요?
 * >아이템의 값을 변경해야 하는 경우에는 Item&을 사용하고, 값을 읽기만 하는 경우에는 const Item&을 사용했습니디.
 * 예를들어 updateItemQuality()에서는 Item의 days와 quality를 변경하므로 Item&을 사용했습니다.그렇지 않은 경우는 const Item&을 썼습니다.
 * 4. 리팩터링 전보다 코드의 의도가 더 잘 드러나는 부분 한 곳을 설명해 주세요.
 * >기존의 cal()함수는 mode값에 따라 가격 품질 만료여부를 모두 계산해 함수의 목적을 한눈에 파악하기 어려웠습니다.
 * 이를 calculateTotalPrice(), countHighQualityItems(), countExpiredItems()로 분리하여 함수 이름만 보아도 각각 어떤 작업을 수행하는지 알 수 있도록 개선했습니다.
 */

#include <iostream>
#include <string>
#include <vector>

struct Item {
    std::string name;
    int days;
    int quality;
    int price;
};

void limitQuality(Item& item) {
    if (item.quality < 0) {
        item.quality = 0;
    }

    if (item.quality > 50) {
        item.quality = 50;
    }
}


void updateItemQuality(Item& item) {
    if (item.name == "Legendary") {
        return;
    }

    if (item.name == "Cheese") {
        item.quality += (item.days > 0) ? 1 : 2;
    }
    else if (item.name == "Ticket") {
        if (item.days > 10) {
            item.quality += 1;
        }
        else if (item.days > 5) {
            item.quality += 2;
        }
        else if (item.days > 0) {
            item.quality += 3;
        }
        else {
            item.quality -= 2;
        }
    }
    else {
        item.quality -= (item.days > 0) ? 1 : 2;
    }

    item.days -= 1;
    limitQuality(item);
}


int calculateTotalPrice(const std::vector<Item>& items) {
    int totalPrice = 0;

    for (const Item& item : items) {
        totalPrice += item.price;
    }

    return totalPrice;
}


int countHighQualityItems(const std::vector<Item>& items) {
    int count = 0;

    for (const Item& item : items) {
        if (item.quality >= 40) {
            count++;
        }
    }

    return count;
}

int countExpiredItems(const std::vector<Item>& items) {
    int count = 0;

    for (const Item& item : items) {
        if (item.days <= 0) {
            count++;
        }
    }

    return count;
}


Item* findItemByName(
    std::vector<Item>& items,
    const std::string& itemName
) {
    for (Item& item : items) {
        if (item.name == itemName) {
            return &item;
        }
    }

    return nullptr;
}


void discountCheesePrice(std::vector<Item>& items) {
    Item* cheese = findItemByName(items, "Cheese");

    if (cheese == nullptr) {
        return;
    }

    cheese->price -= 500;

    if (cheese->price < 0) {
        cheese->price = 0;
    }
}


void updateItemsForOneDay(std::vector<Item>& items) {
    for (Item& item : items) {
        updateItemQuality(item);
    }
}

// 하루의 결과를 출력
void printDailyResult(
    const std::vector<Item>& items,
    int day
) {
    std::cout << "======== Day " << day << " ========" << std::endl;

    for (const Item& item : items) {
        std::cout << item.name
                  << ": days=" << item.days
                  << ", quality=" << item.quality
                  << ", price=" << item.price
                  << std::endl;
    }

    std::cout << "Total price: "
              << calculateTotalPrice(items)
              << std::endl;

    std::cout << "High-quality items: "
              << countHighQualityItems(items)
              << std::endl;

    std::cout << "Expired items: "
              << countExpiredItems(items)
              << std::endl;
}

// 지정된 날짜 수만큼 아이템 상태를 업데이트
void runSimulation(
    std::vector<Item>& items,
    int numberOfDays,
    bool verbose
) {
    for (int day = 1; day <= numberOfDays; ++day) {
        updateItemsForOneDay(items);

        if (verbose) {
            printDailyResult(items, day);
        }
    }
}

int main() {
    std::vector<Item> items = {
        {"Normal", 5, 10, 1000},
        {"Cheese", 3, 20, 3000},
        {"Ticket", 8, 25, 5000},
        {"Legendary", 0, 80, 10000},
        {"Normal", 1, 2, 1500}
    };

    std::cout << "Before update" << std::endl;

    for (const Item& item : items) {
        std::cout << item.name << ": "
                  << item.days << ", "
                  << item.quality
                  << std::endl;
    }

    runSimulation(items, 3, true);

    discountCheesePrice(items);

    Item* cheese = findItemByName(items, "Cheese");

    if (cheese != nullptr) {
        std::cout << "Discounted Cheese price: "
                  << cheese->price
                  << std::endl;
    }

    return 0;
}
