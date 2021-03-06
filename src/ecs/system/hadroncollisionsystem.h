#pragma once

#include <secs/secs.h>

#include "../components.h"

#include <allegro5/allegro_primitives.h>

class HadronCollisionSystem
        : public secs::TypedEntitySystem<HadronCollisionComponent, TransformComponent>,
          public hadron::ICollisionListener
{
public:

    HadronCollisionSystem()
    {
        setStepConfiguration(true, true);
        m_physicsWorld.registerListener(this);
    }

    ~HadronCollisionSystem()
    {
        m_physicsWorld.unregisterListener(this);
    }

    void postUpdate(double delta) override
    {
        SECS_UNUSED(delta);
        m_physicsWorld.step();
    }

    void process(double delta, const secs::Entity &e, HadronCollisionComponent &hcc, TransformComponent& tc) override
    {
        SECS_UNUSED(delta);
        SECS_UNUSED(e);
        const auto& p = tc.position;
        hcc.body->setPosition(hadron::Vec2(p.x() + hcc.offset.x(), p.y() + hcc.offset.y()));
    }

    void render(const secs::Entity &e) override
    {
        auto& hcc = component<HadronCollisionComponent>(e);
        const auto& aabb = hcc.body->aabb();
        float x1, y1, x2, y2;
        x1 = aabb.x;
        x2 = x1 + aabb.width;
        y1 = aabb.y;
        y2 = y1 + aabb.height;
        if( enabled )
        {
            aether::graphics::draw_filled_rectangle(x1, y1, x2, y2, aether::graphics::Color(1.0f, 0.0f, 1.0f));
        }
    }

    void onAdded( const secs::Entity& e )
    {
        auto& hcc = component<HadronCollisionComponent>(e);
        assert(hcc.body != nullptr);
        m_physicsWorld.registerBody(hcc.body);
        secs::Entity* eptr = new secs::Entity(e);
        hcc.body->userData(static_cast<void*>(eptr));
    }

    void onRemoved( const secs::Entity& e )
    {
        auto& hcc = component<HadronCollisionComponent>(e);
        m_physicsWorld.unregisterBody(hcc.body);
        auto eptr = static_cast<secs::Entity*>(hcc.body->userData());
        delete eptr;
        delete hcc.body;
        hcc.body = nullptr;
        printf("cleanup\n"); fflush(0);
    }

    void onCollisionEnter(hadron::Body &b1, hadron::Body &b2, hadron::CollisionResult result) override
    {
        auto e1 = getEntityFromBody(b1);
        auto e2 = getEntityFromBody(b2);
        handleCollision( e1, e2 );
    }

    void onCollisionExit(hadron::Body &b1, hadron::Body &b2) override
    {
        SECS_UNUSED(b1);
        SECS_UNUSED(b2);
        printf("meh\n"); fflush(0);
    }

    void handleCollision( const secs::Entity& e1, const secs::Entity& e2 )
    {
        secs::Entity out1, out2;
        if( entitiesHaveComponents<PlayerInputComponent, CellComponent>(e1, e2, &out1, &out2) )
        {
            auto& cell_comp = component<CellComponent>(out2);
            auto& resources = component<ResourceStorageComponent>(out1);
            switch(cell_comp.type)
            {
            case CellType::IndustryCell: resources.industryCells++; break;
            case CellType::PowerCell: resources.powerCells++; break;
            }
            processor()->removeEntity(out2);

        }
        else if( entitiesHaveComponents<PlayerBulletComponent, EnemyComponent>(e1, e2, &out1, &out2) )
        {
            processor()->removeEntity(out1);
            auto& hc = component<HealthComponent>(out2);
            hc.currentHealth--;
        }
        else if( entitiesHaveComponents<PlayerInputComponent, EnemyComponent>(e1, e2, &out1, &out2) )
        {
            auto& hc = component<HealthComponent>(out1);
            hc.currentHealth = 0;
        }
    }

private:

    template <typename C1, typename C2>
    bool entitiesHaveComponents(secs::Entity e1, secs::Entity e2, secs::Entity* ref1, secs::Entity* ref2)
    {
        bool check = false;
        if( hasComponent<C1>(e1) && hasComponent<C2>(e2) )
        {
            *ref1 = e1; *ref2 = e2;
            check = true;
        }
        else if( hasComponent<C1>(e2) && hasComponent<C2>(e1) )
        {
            *ref1 = e2; *ref2 = e1;
            check = true;
        }
        return check;
    }

    secs::Entity getEntityFromBody( hadron::Body& b )
    {
        return *(static_cast<secs::Entity*>(b.userData()));
    }

    hadron::collision::World m_physicsWorld;

    bool enabled = false;

};
