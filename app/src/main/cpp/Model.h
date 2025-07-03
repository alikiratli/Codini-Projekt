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
    SPACE,      // Weltraum-Thema: Astronaut, Planeten, Sterne
    OCEAN,      // Meeresfrüchte-Thema: Fische, U-Boot, Korallen
    FOREST,     // Wald-Thema: Bäume, Tiere, Blumen
    ROBOT_LAB   // Roboterlabor: Roboter, Maschinen
};

struct Theme {
    ThemeType type;
    std::string backgroundTexture;
    std::string boxTexture;
    std::string targetTexture;
    std::vector<std::string> decorativeElements; // Dekorative Elemente spezifisch für jedes Thema
};

struct UserProgress {
    int currentLevel;
    int totalScore;
    std::map<int, int> levelScores;  // Level-Nummer -> Punkte
<<<<<<< HEAD
    std::map<int, int> levelStars;   // level numarası -> yıldız sayısı (1-3)
=======
    std::map<int, int> levelStars;   // Level-Nummer -> Anzahl Sterne (1-3)
>>>>>>> bf23b1a (4. Commit)
    std::time_t lastPlayTime;
};

struct UserProfile {
    std::string username;
    std::string passwordHash;  // SHA-256 Hash wird verwendet
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
<<<<<<< HEAD
    int baseScore;           // Basis-Level-Punkte
    int optimalCommandCount; // En iyi çözüm için gereken komut sayısı
=======
    int baseScore;           // Grundpunkte des Levels
    int optimalCommandCount; // Benötigte Befehle für optimale Lösung
>>>>>>> bf23b1a (4. Commit)
};

class GameModel {
public:
    GameModel() {
        initializeThemes();
        currentUser = nullptr;
    }

    bool loginUser(const std::string& username, const std::string& password) {
<<<<<<< HEAD
        // Benutzerdaten überprüfen und anmelden
=======
        // Benutzerdaten prüfen und anmelden
>>>>>>> bf23b1a (4. Commit)
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
            return false; // Benutzer existiert bereits
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
<<<<<<< HEAD
            return; // Benutzer ist nicht angemeldet
=======
            return; // Benutzer nicht angemeldet
>>>>>>> bf23b1a (4. Commit)
        }

        currentLevelNumber = levelNumber;
        currentLevel = Level();

<<<<<<< HEAD
        // Für jedes Level ein anderes Theme wählen
=======
        // Verschiedenes Thema für jedes Level wählen
>>>>>>> bf23b1a (4. Commit)
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
        // Dekorative Elemente an zufälligen Positionen hinzufügen
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
        
<<<<<<< HEAD
        // Punkteberechnung
        float timeBonus = std::max(0.0f, 30.0f - timeSpent) * 2;  // 30 saniyeden hızlı bitirme bonusu
        int commandBonus = isOptimal ? 50 : 0;  // Optimal çözüm bonusu
=======
        // Puan hesaplama
        float timeBonus = std::max(0.0f, 30.0f - timeSpent) * 2;  // Bonus für schnellere Fertigstellung als 30 Sekunden
        int commandBonus = isOptimal ? 50 : 0;  // Bonus für optimale Lösung
>>>>>>> bf23b1a (4. Commit)
        
        int score = currentLevel.baseScore + static_cast<int>(timeBonus) + commandBonus;
        
        // Sterne-Berechnung (1-3)
        int stars = 1;  // Mindestens 1 Stern
        if (isOptimal) stars++;  // +1 Stern für optimale Lösung
        if (timeSpent < 20.0f) stars++;  // +1 Stern für schnelle Lösung
        
        // Benutzerfortschritt aktualisieren
        currentUser->progress.levelScores[currentLevelNumber] = score;
        currentUser->progress.levelStars[currentLevelNumber] = stars;
        currentUser->progress.totalScore += score;
        currentUser->progress.lastPlayTime = std::time(nullptr);
        
        if (currentLevelNumber == currentUser->progress.currentLevel) {
            currentUser->progress.currentLevel++;  // Nächstes Level freischalten
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