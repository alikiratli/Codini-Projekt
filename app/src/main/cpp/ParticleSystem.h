#ifndef CODINI_PARTICLE_SYSTEM_H
#define CODINI_PARTICLE_SYSTEM_H

#include <vector>
#include <random>
#include <memory>
#include "Model.h"

struct Particle {
    Vector2 position;
    Vector2 velocity;
    float rotation;
    float rotationSpeed;
    float scale;
    float life;
    float maxLife;
    Vector3 color;
    float alpha;
};

class ParticleEmitter {
public:
    ParticleEmitter(const Vector2& position, const std::string& textureName)
        : position_(position), textureName_(textureName) {
        
        std::random_device rd;
        rng_ = std::mt19937(rd());
    }

    virtual void update(float deltaTime) {
        // Parçacıkları güncelle
        for (auto it = particles_.begin(); it != particles_.end();) {
            updateParticle(*it, deltaTime);
            if (it->life <= 0) {
                it = particles_.erase(it);
            } else {
                ++it;
            }
        }
    }

    virtual void emit(int count) = 0;

    const std::vector<Particle>& getParticles() const { return particles_; }
    const std::string& getTextureName() const { return textureName_; }

protected:
    void updateParticle(Particle& particle, float deltaTime) {
        particle.life -= deltaTime;
        particle.position.x += particle.velocity.x * deltaTime;
        particle.position.y += particle.velocity.y * deltaTime;
        particle.rotation += particle.rotationSpeed * deltaTime;
        particle.alpha = (particle.life / particle.maxLife);
    }

    float random(float min, float max) {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(rng_);
    }

    std::vector<Particle> particles_;
    Vector2 position_;
    std::string textureName_;
    std::mt19937 rng_;
};

// Başarı efekti için yıldız parçacıkları
class StarEmitter : public ParticleEmitter {
public:
    StarEmitter(const Vector2& position) 
        : ParticleEmitter(position, "star_particle.png") {}

    void emit(int count) override {
        for (int i = 0; i < count; i++) {
            Particle p;
            p.position = position_;
            
            // Daire şeklinde dağılım
            float angle = random(0, 2 * M_PI);
            float speed = random(2.0f, 5.0f);
            p.velocity = {cos(angle) * speed, sin(angle) * speed};
            
            p.rotation = random(0, 360);
            p.rotationSpeed = random(-180, 180);
            p.scale = random(0.3f, 0.6f);
            p.life = random(0.5f, 1.5f);
            p.maxLife = p.life;
            p.color = {1.0f, 1.0f, 0.0f}; // Sarı
            p.alpha = 1.0f;
            
            particles_.push_back(p);
        }
    }
};

// Kod bloğu efekti için siber parçacıklar
class CodeParticleEmitter : public ParticleEmitter {
public:
    CodeParticleEmitter(const Vector2& position)
        : ParticleEmitter(position, "code_particle.png") {}

    void emit(int count) override {
        for (int i = 0; i < count; i++) {
            Particle p;
            p.position = position_;
            
            // Yukarı doğru akış
            p.velocity = {random(-1.0f, 1.0f), random(-3.0f, -1.0f)};
            
            p.rotation = 0;
            p.rotationSpeed = 0;
            p.scale = random(0.2f, 0.4f);
            p.life = random(0.8f, 2.0f);
            p.maxLife = p.life;
            p.color = {0.0f, 1.0f, 0.8f}; // Siber mavi
            p.alpha = 1.0f;
            
            particles_.push_back(p);
        }
    }
};

class ParticleSystem {
public:
    void update(float deltaTime) {
        for (auto& emitter : emitters_) {
            emitter->update(deltaTime);
        }
        
        // Boş emitterleri temizle
        emitters_.erase(
            std::remove_if(emitters_.begin(), emitters_.end(),
                [](const std::unique_ptr<ParticleEmitter>& emitter) {
                    return emitter->getParticles().empty();
                }),
            emitters_.end()
        );
    }

    void addStarBurst(const Vector2& position) {
        auto emitter = std::make_unique<StarEmitter>(position);
        emitter->emit(20); // 20 yıldız parçacığı
        emitters_.push_back(std::move(emitter));
    }

    void addCodeEffect(const Vector2& position) {
        auto emitter = std::make_unique<CodeParticleEmitter>(position);
        emitter->emit(10); // 10 kod parçacığı
        emitters_.push_back(std::move(emitter));
    }

    const std::vector<std::unique_ptr<ParticleEmitter>>& getEmitters() const {
        return emitters_;
    }

private:
    std::vector<std::unique_ptr<ParticleEmitter>> emitters_;
};

#endif //CODINI_PARTICLE_SYSTEM_H
