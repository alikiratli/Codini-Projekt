#ifndef ANDROIDGLINVESTIGATIONS_MODEL_H
#define ANDROIDGLINVESTIGATIONS_MODEL_H

#include <vector>
#include "TextureAsset.h"
#include <string>
#include <map>
#include <ctime>

union Vector3 {
    struct {
        float x, y, z;
    };
    float idx[3];
};

union Vector2 {
    struct {
        float x, y;
    };
    struct {
        float u, v;
    };
    float idx[2];
};

struct Vertex {
    constexpr Vertex(const Vector3 &inPosition, const Vector2 &inUV) : position(inPosition),
                                                                       uv(inUV) {}

    Vector3 position;
    Vector2 uv;
};

typedef uint16_t Index;

class Model {
public:
    inline Model(
            std::vector<Vertex> vertices,
            std::vector<Index> indices,
            std::shared_ptr<TextureAsset> spTexture)
            : vertices_(std::move(vertices)),
              indices_(std::move(indices)),
              spTexture_(std::move(spTexture)) {}

    inline const Vertex *getVertexData() const {
        return vertices_.data();
    }

    inline const size_t getIndexCount() const {
        return indices_.size();
    }

    inline const Index *getIndexData() const {
        return indices_.data();
    }

    inline const TextureAsset &getTexture() const {
        return *spTexture_;
    }

private:
    std::vector<Vertex> vertices_;
    std::vector<Index> indices_;
    std::shared_ptr<TextureAsset> spTexture_;
};

struct Position {
    float x;
    float y;
};

struct GameObject {
    Position position;
    float width;
    float height;
    bool isTarget;
    bool isSelected;
};

enum class ThemeType {
    SPACE,      // Uzay teması: astronot, gezegenler, yıldızlar
    OCEAN,      // Deniz teması: balıklar, denizaltı, mercanlar
    FOREST,     // Orman teması: ağaçlar, hayvanlar, çiçekler
    ROBOT_LAB   // Robot laboratuvarı: robotlar, makineler
};

struct Theme {
    ThemeType type;
    std::string backgroundTexture;
    std::string boxTexture;
    std::string targetTexture;
    std::vector<std::string> decorativeElements; // Her temaya özel dekoratif elementler
};

struct UserProgress {
    int currentLevel;
    int totalScore;
    std::map<int, int> levelScores;  // level numarası -> puan
    std::map<int, int> levelStars;   // level numarası -> yıldız sayısı (1-3)
    std::time_t lastPlayTime;
};

struct UserProfile {
    std::string username;
    std::string passwordHash;  // SHA-256 hash kullanılacak
    UserProgress progress;
    bool isLoggedIn;
};

struct LevelCompletion {
    int score;
    int stars;
    int commandsUsed;
    float timeSpent;
    bool isOptimalSolution;
};

struct Level {
    std::vector<GameObject> boxes;
    std::vector<GameObject> targets;
    std::vector<std::string> availableCommands;
    int minCommandCount;
    std::string description;
    Theme theme;
    std::vector<GameObject> decorations;
    int baseScore;           // Temel seviye puanı
    int optimalCommandCount; // En iyi çözüm için gereken komut sayısı
};

class GameModel {
public:
    GameModel() {
        initializeThemes();
        currentUser = nullptr;
    }

    bool loginUser(const std::string& username, const std::string& password) {
        // Kullanıcı bilgilerini kontrol et ve giriş yap
        std::string hashedPassword = hashPassword(password);
        if (users_.find(username) != users_.end() && 
            users_[username].passwordHash == hashedPassword) {
            currentUser = &users_[username];
            currentUser->isLoggedIn = true;
            loadUserProgress();
            return true;
        }
        return false;
    }

    bool registerUser(const std::string& username, const std::string& password) {
        if (users_.find(username) != users_.end()) {
            return false; // Kullanıcı zaten var
        }

        UserProfile newUser;
        newUser.username = username;
        newUser.passwordHash = hashPassword(password);
        newUser.isLoggedIn = false;
        newUser.progress = UserProgress{1, 0, {}, {}, std::time(nullptr)};
        
        users_[username] = newUser;
        return true;
    }

    void initializeLevel(int levelNumber) {
        if (!currentUser || !currentUser->isLoggedIn) {
            return; // Kullanıcı girişi yapılmamış
        }

        currentLevelNumber = levelNumber;
        currentLevel = Level();

        // Her seviye için farklı tema seç
        ThemeType levelTheme;
        switch(levelNumber % 4) {
            case 0: levelTheme = ThemeType::SPACE; break;
            case 1: levelTheme = ThemeType::OCEAN; break;
            case 2: levelTheme = ThemeType::FOREST; break;
            case 3: levelTheme = ThemeType::ROBOT_LAB; break;
        }
        currentLevel.theme = themes_[levelTheme];

        switch(levelNumber) {
            case 1:
                currentLevel.description = "Bewege die Box zum roten Punkt!";
                currentLevel.availableCommands = {"vorwärts", "rechts", "links"};
                currentLevel.minCommandCount = 3;
                currentLevel.baseScore = 100;
                currentLevel.optimalCommandCount = 3;
                currentLevel.boxes.push_back({Position{0.0f, 0.0f}, 1.0f, 1.0f, false, false});
                currentLevel.targets.push_back({Position{3.0f, 3.0f}, 0.5f, 0.5f, true, false});
                addDecorativeElements(3);
                break;
            case 2:
                currentLevel.description = "Bewege zwei Boxen zu ihren Zielpunkten!";
                currentLevel.availableCommands = {"vorwärts", "rechts", "links", "wiederholen"};
                currentLevel.minCommandCount = 5;
                currentLevel.baseScore = 200;
                currentLevel.optimalCommandCount = 5;
                currentLevel.boxes.push_back({Position{0.0f, 0.0f}, 1.0f, 1.0f, false, false});
                currentLevel.boxes.push_back({Position{0.0f, 2.0f}, 1.0f, 1.0f, false, false});
                currentLevel.targets.push_back({Position{4.0f, 0.0f}, 0.5f, 0.5f, true, false});
                currentLevel.targets.push_back({Position{4.0f, 2.0f}, 0.5f, 0.5f, true, false});
                addDecorativeElements(4);
                break;
            case 3:
                currentLevel.description = "Erstelle eine Schleife um die Boxen effizient zu bewegen!";
                currentLevel.availableCommands = {"vorwärts", "rechts", "links", "wiederholen", "schleife"};
                currentLevel.minCommandCount = 4;
                currentLevel.baseScore = 300;
                currentLevel.optimalCommandCount = 4;
                currentLevel.boxes.push_back({Position{0.0f, 0.0f}, 1.0f, 1.0f, false, false});
                currentLevel.boxes.push_back({Position{0.0f, 2.0f}, 1.0f, 1.0f, false, false});
                currentLevel.boxes.push_back({Position{0.0f, 4.0f}, 1.0f, 1.0f, false, false});
                currentLevel.targets.push_back({Position{5.0f, 0.0f}, 0.5f, 0.5f, true, false});
                currentLevel.targets.push_back({Position{5.0f, 2.0f}, 0.5f, 0.5f, true, false});
                currentLevel.targets.push_back({Position{5.0f, 4.0f}, 0.5f, 0.5f, true, false});
                addDecorativeElements(5);
                break;
        }
    }

    void addDecorativeElements(int count) {
        // Rastgele pozisyonlarda dekoratif elementler ekle
        for(int i = 0; i < count; i++) {
            float x = static_cast<float>(rand() % 8);
            float y = static_cast<float>(rand() % 8);
            currentLevel.decorations.push_back({
                Position{x, y},
                0.5f, 0.5f, false, false
            });
        }
    }

    LevelCompletion completeLevelWithSolution(const std::vector<std::string>& commands, float timeSpent) {
        if (!currentUser || !currentUser->isLoggedIn) {
            return LevelCompletion{0, 0, 0, 0.0f, false};
        }

        int commandCount = commands.size();
        bool isOptimal = commandCount <= currentLevel.optimalCommandCount;
        
        // Puan hesaplama
        float timeBonus = std::max(0.0f, 30.0f - timeSpent) * 2;  // 30 saniyeden hızlı bitirme bonusu
        int commandBonus = isOptimal ? 50 : 0;  // Optimal çözüm bonusu
        
        int score = currentLevel.baseScore + static_cast<int>(timeBonus) + commandBonus;
        
        // Yıldız hesaplama (1-3 arası)
        int stars = 1;  // En az 1 yıldız
        if (isOptimal) stars++;  // Optimal çözüm için +1 yıldız
        if (timeSpent < 20.0f) stars++;  // Hızlı çözüm için +1 yıldız
        
        // Kullanıcı ilerlemesini güncelle
        currentUser->progress.levelScores[currentLevelNumber] = score;
        currentUser->progress.levelStars[currentLevelNumber] = stars;
        currentUser->progress.totalScore += score;
        currentUser->progress.lastPlayTime = std::time(nullptr);
        
        if (currentLevelNumber == currentUser->progress.currentLevel) {
            currentUser->progress.currentLevel++;  // Bir sonraki seviyeyi aç
        }
        
        saveUserProgress();  // İlerlemeyi kaydet
        
        return LevelCompletion{score, stars, commandCount, timeSpent, isOptimal};
    }

    // Getter metodları
    bool isUserLoggedIn() const { return currentUser && currentUser->isLoggedIn; }
    const UserProgress* getCurrentProgress() const { 
        return currentUser ? &currentUser->progress : nullptr; 
    }
    int getCurrentLevel() const { 
        return currentUser ? currentUser->progress.currentLevel : 1; 
    }
    int getTotalScore() const { 
        return currentUser ? currentUser->progress.totalScore : 0; 
    }

private:
    void initializeThemes() {
        themes_[ThemeType::SPACE] = {
            ThemeType::SPACE,
            "space_background.png",
            "astronaut.png",
            "planet.png",
            {"star.png", "meteor.png", "satellite.png"}
        };
        
        themes_[ThemeType::OCEAN] = {
            ThemeType::OCEAN,
            "ocean_background.png",
            "submarine.png",
            "treasure.png",
            {"fish.png", "coral.png", "seaweed.png"}
        };
        
        themes_[ThemeType::FOREST] = {
            ThemeType::FOREST,
            "forest_background.png",
            "explorer.png",
            "tree_house.png",
            {"tree.png", "flower.png", "mushroom.png"}
        };
        
        themes_[ThemeType::ROBOT_LAB] = {
            ThemeType::ROBOT_LAB,
            "lab_background.png",
            "robot.png",
            "charging_station.png",
            {"gear.png", "circuit.png", "screen.png"}
        };
    }

    std::string hashPassword(const std::string& password) {
        // TODO: Implement SHA-256 hashing
        return password; // Şimdilik basit impl.
    }

    void loadUserProgress() {
        // TODO: Implement loading from persistent storage
    }

    void saveUserProgress() {
        // TODO: Implement saving to persistent storage
    }

    Level currentLevel;
    int currentLevelNumber;
    std::map<ThemeType, Theme> themes_;
    std::map<std::string, UserProfile> users_;
    UserProfile* currentUser;
};

#endif //ANDROIDGLINVESTIGATIONS_MODEL_H