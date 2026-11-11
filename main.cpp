#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;

// Hides the cube that rides inside the ship / UFO / jetpack.
//
// The cube is hidden only while the game layer is being drawn, then put back
// right after. GD keeps re-showing the sprite during its own update, so changing
// it there doesn't stick. Hiding it at draw time always wins, and the game's
// real state is never changed, so switching modes can't break anything.

static bool setting(char const* key) {
    return Mod::get()->getSettingValue<bool>(key);
}

static bool shouldHide(PlayerObject* p) {
    if (!p || !p->m_iconSprite) return false;
    if (p->m_isShip) return setting(p->m_isPlatformer ? "hide-in-jetpack" : "hide-in-ship");
    if (p->m_isBird) return setting("hide-in-ufo");
    return false;
}

struct HiddenSprite {
    CCNode* node;
    bool wasVisible;
};

class $modify(NoRiderLayer, GJBaseGameLayer) {
    void visit() {
        // Up to 2 players x (cube + glow)
        HiddenSprite hidden[4];
        int count = 0;

        for (auto p : { m_player1, m_player2 }) {
            if (!shouldHide(p)) continue;

            // Secondary color + whitener are children of m_iconSprite, so hiding it hides them too.
            hidden[count++] = { p->m_iconSprite, p->m_iconSprite->isVisible() };
            p->m_iconSprite->setVisible(false);

            if (p->m_iconGlow && p->m_iconGlow->getParent() != p->m_iconSprite) {
                hidden[count++] = { p->m_iconGlow, p->m_iconGlow->isVisible() };
                p->m_iconGlow->setVisible(false);
            }
        }

        GJBaseGameLayer::visit();

        // Put everything back exactly how GD had it.
        for (int i = 0; i < count; i++) {
            hidden[i].node->setVisible(hidden[i].wasVisible);
        }
    }
};
