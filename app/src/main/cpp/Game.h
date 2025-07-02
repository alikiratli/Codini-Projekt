#ifndef CODINI_GAME_H
#define CODINI_GAME_H

#include "Model.h"
#include "Renderer.h"
#include "ParticleSystem.h"
#include "AudioManager.h"  // Ses yönetimi için header
#include <memory>
#include <vector>
#include <queue>
#include <cmath>

// Spielzustände
enum class GameState {
    MENU,           // Hauptmenü
    LEVEL_SELECT,   // Levelauswahl
    PLAYING,        // Aktives Spiel
    CODING,         // Code-Editor aktiv
    ANIMATING,      // Animation läuft
    LEVEL_COMPLETE, // Level geschafft
    PAUSED         // Pause
};

// Grundlegende Befehle (Level 1)
enum class CommandType {
    // Level 1 - Grundbewegungen
    MOVE_FORWARD,    // Vorwärts bewegen
    TURN_LEFT,       // Links drehen
    TURN_RIGHT,      // Rechts drehen
    
    // Level 2 - Schleifen
    LOOP_START,      // Schleifenbeginn
    LOOP_END,        // Schleifenende
    
    // Level 3 - Funktionen
    FUNCTION_DEF,    // Funktionsdefinition
    FUNCTION_CALL,   // Funktionsaufruf
    
    // Level 4 - Bedingungen
    IF_PATH_AHEAD,   // Wenn Weg voraus
    IF_TARGET_NEARBY,// Wenn Ziel in der Nähe
    IF_END,          // Ende der If-Anweisung
    
    // Level 5 - Erweiterte Bewegungen
    JUMP,           // Springen
    MOVE_BACKWARD,  // Rückwärts bewegen
    PICK_ITEM,      // Gegenstand aufheben
    USE_ITEM,       // Gegenstand benutzen
    
    // Level 6 - Spezielle Befehle
    TELEPORT,       // Teleportieren
    CREATE_BRIDGE,   // Brücke erstellen
    ACTIVATE_SWITCH  // Schalter aktivieren
};

// Komut grupları için yapı
struct CommandGroup {
    std::string name;               // Grup adı
    std::string description;        // Açıklama
    std::vector<CommandType> commands; // Gruptaki komutlar
    int unlockedAtLevel;           // Auf welchem Level wird freigeschaltet
};

class Game {
public:
    Game(android_app* app) : app_(app), gameState_(GameState::MENU) {
        model_ = std::make_unique<GameModel>();
        renderer_ = std::make_unique<Renderer>(app);
        particleSystem_ = std::make_unique<ParticleSystem>();
        audioManager_ = std::make_unique<AudioManager>();
        initializeCommandGroups();
        initializeGame();
        loadSoundEffects();
    }

    void initializeGame() {
        currentLevel_ = 1;
        model_->initializeLevel(currentLevel_);
        commandQueue_.clear();
    }

    void update(float deltaTime) {
        // Parçacık sistemini güncelle
        particleSystem_->update(deltaTime);

        switch (gameState_) {
            case GameState::PLAYING:
                updateGameplay(deltaTime);
                break;
            case GameState::ANIMATING:
                updateAnimation(deltaTime);
                break;
            case GameState::CODING:
                updateCodeExecution(deltaTime);
                break;
            default:
                break;
        }
    }

    void handleInput(const GameActivityMotionEvent* event) {
        switch (gameState_) {
            case GameState::MENU:
                handleMenuInput(event);
                break;
            case GameState::CODING:
                handleCodingInput(event);
                break;
            case GameState::PLAYING:
                handleGameplayInput(event);
                break;
            default:
                break;
        }
    }

    void render() {
        renderer_->beginFrame();

        // Hintergrund basierend auf aktuellem Theme rendern
        renderer_->renderBackground(model_->getCurrentTheme());

        // Spielobjekte rendern
        for (const auto& box : model_->getBoxes()) {
            renderer_->renderBox(box);
        }
        for (const auto& target : model_->getTargets()) {
            renderer_->renderTarget(target);
        }
        for (const auto& decoration : model_->getDecorations()) {
            renderer_->renderDecoration(decoration);
        }

        // Parçacık efektlerini render et
        for (const auto& emitter : particleSystem_->getEmitters()) {
            for (const auto& particle : emitter->getParticles()) {
                renderer_->renderParticle(particle, emitter->getTextureName());
            }
        }

        // UI-Elemente basierend auf GameState rendern
        switch (gameState_) {
            case GameState::MENU:
                renderMenu();
                break;
            case GameState::CODING:
                renderCodingInterface();
                break;
            case GameState::LEVEL_COMPLETE:
                renderLevelComplete();
                break;
            default:
                break;
        }

        renderer_->endFrame();
    }

private:
    void initializeCommandGroups() {
        commandGroups_ = {
            // Level 1 - Grundbewegung
            CommandGroup{
                "Temel Bewegungen",
                "Lerne die grundlegenden Bewegungsbefehle",
                {CommandType::MOVE_FORWARD, CommandType::TURN_LEFT, CommandType::TURN_RIGHT},
                1
            },
            
            // Level 2 - Schleifen
            CommandGroup{
                "Schleifen",
                "Wiederhole Aktionen mit Schleifen",
                {CommandType::LOOP_START, CommandType::LOOP_END},
                2
            },
            
            // Level 3 - Funktionen
            CommandGroup{
                "Funktionen",
                "Erstelle und verwende eigene Funktionen",
                {CommandType::FUNCTION_DEF, CommandType::FUNCTION_CALL},
                3
            },
            
            // Level 4 - Bedingungen
            CommandGroup{
                "Bedingungen",
                "Triff Entscheidungen basierend auf der Umgebung",
                {CommandType::IF_PATH_AHEAD, CommandType::IF_TARGET_NEARBY, CommandType::IF_END},
                4
            },
            
            // Level 5 - Erweiterte Bewegungen
            CommandGroup{
                "Erweiterte Bewegungen",
                "Nutze fortgeschrittene Bewegungsbefehle",
                {CommandType::JUMP, CommandType::MOVE_BACKWARD, CommandType::PICK_ITEM, CommandType::USE_ITEM},
                5
            },
            
            // Level 6 - Spezielle Befehle
            CommandGroup{
                "Spezielle Aktionen",
                "Verwende spezielle Fähigkeiten",
                {CommandType::TELEPORT, CommandType::CREATE_BRIDGE, CommandType::ACTIVATE_SWITCH},
                6
            }
        };
    }

    void updateGameplay(float deltaTime) {
        // Spiellogik-Update
        if (!commandQueue_.empty() && !isAnimating_) {
            executeNextCommand();
        }
        
        // Kollisionserkennung
        checkCollisions();
        
        // Überprüfen, ob Level abgeschlossen
        if (model_->checkWinCondition()) {
            completeLevelWithSolution();
        }
    }

    void updateAnimation(float deltaTime) {
        if (currentAnimation_) {
            if (currentAnimation_->update(deltaTime)) {
                currentAnimation_ = nullptr;
                isAnimating_ = false;
                if (commandQueue_.empty()) {
                    checkLevelCompletion();
                }
            }
        }
    }

    void updateCodeExecution(float deltaTime) {
        executionTimer_ += deltaTime;
        if (executionTimer_ >= commandExecutionInterval_) {
            executionTimer_ = 0.0f;
            if (!commandQueue_.empty()) {
                executeNextCommand();
            }
        }
    }

    std::vector<CommandType> getAvailableCommands() {
        std::vector<CommandType> available;
        
        // Verfügbare Befehle für das aktuelle Level sammeln
        for (const auto& group : commandGroups_) {
            if (group.unlockedAtLevel <= currentLevel_) {
                available.insert(available.end(), 
                               group.commands.begin(), 
                               group.commands.end());
            }
        }
        
        return available;
    }

    void executeNextCommand() {
        if (commandQueue_.empty()) return;
        
        Command cmd = commandQueue_.front();
        commandQueue_.pop();
        
        // Komut yürütüldüğünde efekt
        if (auto* box = getSelectedBox()) {
            particleSystem_->addCodeEffect(Vector2{box->position.x, box->position.y});
        }
        
        switch (cmd.type) {
            // Temel Hareket Komutları
            case CommandType::MOVE_FORWARD:
                moveSelectedBox();
                break;
            case CommandType::TURN_LEFT:
                rotateSelectedBox(-90.0f);
                break;
            case CommandType::TURN_RIGHT:
                rotateSelectedBox(90.0f);
                break;
                
            // Döngü Komutları
            case CommandType::LOOP_START:
                pushLoopContext(cmd.loopCount);
                break;
            case CommandType::LOOP_END:
                processLoopEnd();
                break;
                
            // Koşul Komutları
            case CommandType::IF_PATH_AHEAD:
                executeIfPathAhead();
                break;
            case CommandType::IF_TARGET_NEARBY:
                executeIfTargetNearby();
                break;
            case CommandType::IF_END:
                processIfEnd();
                break;
                
            // Gelişmiş Hareket Komutları
            case CommandType::JUMP:
                executeJump();
                break;
            case CommandType::MOVE_BACKWARD:
                moveSelectedBoxBackward();
                break;
            case CommandType::PICK_ITEM:
                executePickItem();
                break;
            case CommandType::USE_ITEM:
                executeUseItem();
                break;
                
            // Özel Komutlar
            case CommandType::TELEPORT:
                executeTeleport();
                break;
            case CommandType::CREATE_BRIDGE:
                executeCreateBridge();
                break;
            case CommandType::ACTIVATE_SWITCH:
                executeActivateSwitch();
                break;
        }
    }

    // Yeni komut uygulamaları
    void executeIfPathAhead() {
        if (auto* box = getSelectedBox()) {
            float angle = box->rotation * M_PI / 180.0f;
            float checkX = box->position.x + cos(angle);
            float checkY = box->position.y + sin(angle);
            
            if (isValidPosition(checkX, checkY)) {
                // Yol açıksa sonraki komutu işle
                executeNextCommand();
            } else {
                // Yol kapalıysa if bloğunu atla
                skipToIfEnd();
            }
        }
    }

    void executeIfTargetNearby() {
        if (auto* box = getSelectedBox()) {
            bool targetNearby = false;
            
            // En yakın hedefi kontrol et
            for (const auto& target : model_->getTargets()) {
                float dx = target.position.x - box->position.x;
                float dy = target.position.y - box->position.y;
                float distance = sqrt(dx*dx + dy*dy);
                
                if (distance < 2.0f) { // 2 birim mesafe içinde
                    targetNearby = true;
                    break;
                }
            }
            
            if (targetNearby) {
                executeNextCommand();
            } else {
                skipToIfEnd();
            }
        }
    }

    void executeJump() {
        if (auto* box = getSelectedBox()) {
            float angle = box->rotation * M_PI / 180.0f;
            float targetX = box->position.x + 2.0f * cos(angle);
            float targetY = box->position.y + 2.0f * sin(angle);
            
            // Sprunganimation mit Sound starten
            audioManager_->playSound("jump", 1.0f);
            currentAnimation_ = std::make_unique<JumpAnimation>(*box, targetX, targetY, 0.5f);
            isAnimating_ = true;
        }
    }

    void executeTeleport() {
        if (auto* box = getSelectedBox()) {
            // Ersten Teleport-Sound abspielen
            audioManager_->playSound("teleport_start", 0.8f);
            
            float angle = box->rotation * M_PI / 180.0f;
            float targetX = box->position.x + 3.0f * cos(angle);
            float targetY = box->position.y + 3.0f * sin(angle);
            
            currentAnimation_ = std::make_unique<TeleportAnimation>(*box, targetX, targetY, 0.7f);
            isAnimating_ = true;
            
            // Zweiten Teleport-Sound mit Verzögerung abspielen
            audioManager_->playSound("teleport_end", 0.8f, 0.35f);
        }
    }

    void checkCollisions() {
        // Kollisionserkennung zwischen Boxen und Hindernissen
        for (auto& box : model_->getBoxes()) {
            for (const auto& target : model_->getTargets()) {
                if (checkBoxTargetCollision(box, target)) {
                    // Kollisionsbehandlung
                }
            }
        }
    }

    bool checkBoxTargetCollision(const GameObject& box, const GameObject& target) {
        // AABB Kollisionserkennung
        return (box.position.x < target.position.x + target.width &&
                box.position.x + box.width > target.position.x &&
                box.position.y < target.position.y + target.height &&
                box.position.y + box.height > target.position.y);
    }

    void completeLevelWithSolution() {
        gameState_ = GameState::LEVEL_COMPLETE;
        LevelCompletion completion = model_->completeLevelWithSolution(
            getCurrentCommandList(), 
            executionTimer_
        );

        // Başarı yıldız efektleri
        for (const auto& target : model_->getTargets()) {
            particleSystem_->addStarBurst(Vector2{target.position.x, target.position.y});
        }

        // Level tamamlama animasyonu
        startLevelCompleteAnimation(completion);
    }

    std::vector<std::string> getCurrentCommandList() {
        std::vector<std::string> commands;
        // Konvertiere ausgeführte Befehle in Strings
        return commands;
    }

    android_app* app_;
    std::unique_ptr<GameModel> model_;
    std::unique_ptr<Renderer> renderer_;
    std::unique_ptr<ParticleSystem> particleSystem_;
    std::unique_ptr<AudioManager> audioManager_;
    GameState gameState_;
    int currentLevel_;
    std::queue<Command> commandQueue_;
    bool isAnimating_ = false;
    float executionTimer_ = 0.0f;
    const float commandExecutionInterval_ = 0.5f; // Sekunden zwischen Befehlen
    std::unique_ptr<Animation> currentAnimation_;
    std::vector<std::pair<int, int>> loopStack_; // Loop-Kontext (Zähler, Start-Index)

    // Animation-System
    class Animation {
    public:
        virtual bool update(float deltaTime) = 0;
        virtual ~Animation() = default;
    };

    class MoveAnimation : public Animation {
    public:
        MoveAnimation(GameObject& obj, float targetX, float targetY, float duration)
            : object_(obj), 
              startX_(obj.position.x), startY_(obj.position.y),
              targetX_(targetX), targetY_(targetY),
              duration_(duration), currentTime_(0) {}

        bool update(float deltaTime) override {
            currentTime_ += deltaTime;
            float progress = std::min(currentTime_ / duration_, 1.0f);
            
            // Easing-Funktion für smoothe Bewegung
            progress = easeInOutQuad(progress);
            
            object_.position.x = startX_ + (targetX_ - startX_) * progress;
            object_.position.y = startY_ + (targetY_ - startY_) * progress;
            
            return progress >= 1.0f;
        }

    private:
        float easeInOutQuad(float t) {
            return t < 0.5f ? 2.0f * t * t : -1.0f + (4.0f - 2.0f * t) * t;
        }

        GameObject& object_;
        float startX_, startY_;
        float targetX_, targetY_;
        float duration_;
        float currentTime_;
    };

    class RotateAnimation : public Animation {
    public:
        RotateAnimation(GameObject& obj, float targetAngle, float duration)
            : object_(obj),
              startAngle_(obj.rotation),
              targetAngle_(targetAngle),
              duration_(duration), currentTime_(0) {}

        bool update(float deltaTime) override {
            currentTime_ += deltaTime;
            float progress = std::min(currentTime_ / duration_, 1.0f);
            
            // Easing-Funktion für smoothe Rotation
            progress = easeInOutQuad(progress);
            
            object_.rotation = startAngle_ + (targetAngle_ - startAngle_) * progress;
            return progress >= 1.0f;
        }

    private:
        float easeInOutQuad(float t) {
            return t < 0.5f ? 2.0f * t * t : -1.0f + (4.0f - 2.0f * t) * t;
        }

        GameObject& object_;
        float startAngle_;
        float targetAngle_;
        float duration_;
        float currentTime_;
    };

    class JumpAnimation : public Animation {
    public:
        JumpAnimation(GameObject& obj, float targetX, float targetY, float duration)
            : object_(obj), 
              startX_(obj.position.x), startY_(obj.position.y),
              targetX_(targetX), targetY_(targetY),
              duration_(duration), currentTime_(0) {}

        bool update(float deltaTime) override {
            currentTime_ += deltaTime;
            float progress = std::min(currentTime_ / duration_, 1.0f);
            
            // Easing für geschmeidigere Bewegung
            float easedProgress = easeOutBack(progress);
            
            // Parabelförmige Sprungbewegung mit verstärktem Höheneffekt
            float jumpHeight = 2.0f; // Erhöhte maximale Sprunghöhe
            float verticalProgress = sin(easedProgress * M_PI); // 0 -> 1 -> 0
            verticalProgress = easeOutQuad(verticalProgress); // Sanftere Landung
            
            // Horizontale Bewegung mit leichtem "Überschwingen"
            object_.position.x = startX_ + (targetX_ - startX_) * easedProgress;
            object_.position.y = startY_ + (targetY_ - startY_) * easedProgress;
            object_.position.z = jumpHeight * verticalProgress;
            
            // Partikeleffekt während des Sprungs
            if (verticalProgress > 0.7f) {
                particleSystem_->addJumpEffect(Vector2{object_.position.x, object_.position.y});
            }
            
            return progress >= 1.0f;
        }

    private:
        float easeOutBack(float t) {
            const float c1 = 1.70158f;
            const float c3 = c1 + 1.0f;
            return 1.0f + c3 * pow(t - 1.0f, 3.0f) + c1 * pow(t - 1.0f, 2.0f);
        }
        
        float easeOutQuad(float t) {
            return 1.0f - (1.0f - t) * (1.0f - t);
        }

        GameObject& object_;
        float startX_, startY_;
        float targetX_, targetY_;
        float duration_;
        float currentTime_;
    };

    class TeleportAnimation : public Animation {
    public:
        TeleportAnimation(GameObject& obj, float targetX, float targetY, float duration)
            : object_(obj), 
              startX_(obj.position.x), startY_(obj.position.y),
              targetX_(targetX), targetY_(targetY),
              duration_(duration), currentTime_(0) {
            // Initialer Teleport-Effekt an Startposition
            particleSystem_->addTeleportEffect(Vector2{startX_, startY_}, true);
        }

        bool update(float deltaTime) override {
            currentTime_ += deltaTime;
            float progress = std::min(currentTime_ / duration_, 1.0f);
            
            // Easing für geschmeidigere Animation
            float easedProgress = easeInOutExpo(progress);
            
            // Teleporteffekt: Erst schrumpfen, dann wachsen mit Farbeffekt
            if (progress < 0.5f) {
                // Schrumpfen und verblassen
                float scale = 1.0f - easedProgress * 2.0f;
                object_.scale = scale;
                object_.alpha = scale; // Transparenz hinzufügen
                
                // Kontinuierliche Partikel während des Verschwindens
                if (progress < 0.4f) {
                    particleSystem_->addTeleportTrail(
                        Vector2{object_.position.x, object_.position.y}
                    );
                }
            } else {
                // An neuer Position erscheinen
                if (!hasTelepported_) {
                    object_.position.x = targetX_;
                    object_.position.y = targetY_;
                    // Teleport-Effekt an Zielposition
                    particleSystem_->addTeleportEffect(
                        Vector2{targetX_, targetY_}, 
                        false
                    );
                    hasTelepported_ = true;
                }
                
                // Wachsen und erscheinen
                float scale = (easedProgress - 0.5f) * 2.0f;
                object_.scale = scale;
                object_.alpha = scale;
            }
            
            return progress >= 1.0f;
        }

    private:
        float easeInOutExpo(float t) {
            if (t == 0.0f || t == 1.0f) return t;
            
            if (t < 0.5f) {
                return pow(2.0f, 20.0f * t - 10.0f) / 2.0f;
            }
            return (2.0f - pow(2.0f, -20.0f * t + 10.0f)) / 2.0f;
        }

        GameObject& object_;
        float startX_, startY_;
        float targetX_, targetY_;
        float duration_;
        float currentTime_;
        bool hasTelepported_ = false;
    };

    void moveSelectedBox() {
        if (auto* box = getSelectedBox()) {
            // Bewegungsrichtung basierend auf Rotation berechnen
            float angle = box->rotation * M_PI / 180.0f;
            float targetX = box->position.x + cos(angle);
            float targetY = box->position.y + sin(angle);
            
            // Kollisionsprüfung für neue Position
            if (isValidPosition(targetX, targetY)) {
                currentAnimation_ = std::make_unique<MoveAnimation>(*box, targetX, targetY, 0.3f);
                isAnimating_ = true;
            }
        }
    }

    void rotateSelectedBox(float angle) {
        if (auto* box = getSelectedBox()) {
            float targetAngle = box->rotation + angle;
            currentAnimation_ = std::make_unique<RotateAnimation>(*box, targetAngle, 0.2f);
            isAnimating_ = true;
        }
    }

    GameObject* getSelectedBox() {
        auto& boxes = model_->getBoxes();
        auto it = std::find_if(boxes.begin(), boxes.end(),
                             [](const GameObject& box) { return box.isSelected; });
        return it != boxes.end() ? &(*it) : nullptr;
    }

    bool isValidPosition(float x, float y) {
        // Spielfeldgrenzen prüfen
        if (x < 0 || x > FIELD_WIDTH || y < 0 || y > FIELD_HEIGHT) {
            return false;
        }

        // Kollision mit anderen Boxen prüfen
        const auto& boxes = model_->getBoxes();
        for (const auto& box : boxes) {
            if (!box.isSelected && // Ignoriere ausgewählte Box
                std::abs(x - box.position.x) < 1.0f &&
                std::abs(y - box.position.y) < 1.0f) {
                return false;
            }
        }

        return true;
    }

    void handleMenuInput(const GameActivityMotionEvent* event) {
        // Touch-Position in Menü-Koordinaten umwandeln
        float touchX = event->positions[0].x;
        float touchY = event->positions[0].y;
        
        // Prüfen, ob ein Menü-Button getroffen wurde
        if (event->action == AMOTION_EVENT_ACTION_UP) {
            if (checkMenuButtonCollision(touchX, touchY, "start")) {
                gameState_ = GameState::LEVEL_SELECT;
            } else if (checkMenuButtonCollision(touchX, touchY, "options")) {
                // Optionen öffnen
            }
        }
    }

    void handleCodingInput(const GameActivityMotionEvent* event) {
        float touchX = event->positions[0].x;
        float touchY = event->positions[0].y;

        if (event->action == AMOTION_EVENT_ACTION_UP) {
            // Prüfen, ob ein Befehlsbutton getroffen wurde
            for (const auto& command : model_->getCommands()) {
                if (checkCommandButtonCollision(touchX, touchY, command)) {
                    addCommandToQueue(command);
                    break;
                }
            }
            
            // Play/Stop/Reset Buttons
            if (checkButtonCollision(touchX, touchY, playButtonRect_)) {
                startCodeExecution();
            } else if (checkButtonCollision(touchX, touchY, stopButtonRect_)) {
                stopCodeExecution();
            } else if (checkButtonCollision(touchX, touchY, resetButtonRect_)) {
                resetLevel();
            }
        }
    }

    void startCodeExecution() {
        if (gameState_ != GameState::CODING || !commandQueue_.empty()) {
            return;
        }
        
        gameState_ = GameState::PLAYING;
        executionTimer_ = 0.0f;
        executeNextCommand();
    }

    void stopCodeExecution() {
        if (gameState_ != GameState::PLAYING) {
            return;
        }
        
        gameState_ = GameState::CODING;
        commandQueue_ = std::queue<Command>(); // Queue leeren
        resetBoxPositions();
    }

    void resetLevel() {
        stopCodeExecution();
        model_->initializeLevel(currentLevel_);
        commandQueue_ = std::queue<Command>();
        isAnimating_ = false;
        currentAnimation_ = nullptr;
        executionTimer_ = 0.0f;
    }

    void loadSoundEffects() {
        audioManager_->loadSound("jump", "sounds/jump.wav");
        audioManager_->loadSound("teleport_start", "sounds/teleport_start.wav");
        audioManager_->loadSound("teleport_end", "sounds/teleport_end.wav");
        audioManager_->loadSound("move", "sounds/move.wav");
        audioManager_->loadSound("rotate", "sounds/rotate.wav");
        audioManager_->loadSound("success", "sounds/success.wav");
        audioManager_->loadSound("error", "sounds/error.wav");
    }

    static constexpr float FIELD_WIDTH = 8.0f;
    static constexpr float FIELD_HEIGHT = 8.0f;
    Rect playButtonRect_;
    Rect stopButtonRect_;
    Rect resetButtonRect_;
};

#endif //CODINI_GAME_H
