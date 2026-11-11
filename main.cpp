#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

// Hides the cube that rides inside the ship / UFO / jetpack.
//
// How it works:
//  - Entering ship or UFO mode -> hide the cube sprite (and its glow).
//  - Leaving ship or UFO mode  -> show it again BEFORE the game switches to the
//    next mode, so modes like robot/spider can still hide it themselves.
//  - Every frame while in ship/UFO -> keep it hidden, in case the game re-shows it.

static bool setting(char const* key) {
    return Mod::get()->getSettingValue<bool>(key);
}

class $modify(NoRiderPlayer, PlayerObject) {
    struct Fields {
        bool m_riderHidden = false;
        bool m_glowWasVisible = false;
    };

    bool inHideMode() {
        if (m_isShip) return setting(m_isPlatformer ? "hide-in-jetpack" : "hide-in-ship");
        if (m_isBird) return setting("hide-in-ufo");
        return false;
    }

    void hideRider() {
        if (!m_iconSprite) return;
        auto fields = m_fields.self();
        if (!fields->m_riderHidden) {
            fields->m_glowWasVisible = m_iconGlow && m_iconGlow->isVisible();
            fields->m_riderHidden = true;
        }
        // Secondary color + whitener are children of m_iconSprite, so hiding it hides them too.
        m_iconSprite->setVisible(false);
        if (m_iconGlow && m_iconGlow->getParent() != m_iconSprite) {
            m_iconGlow->setVisible(false);
        }
    }

    void showRider() {
        auto fields = m_fields.self();
        if (!fields->m_riderHidden) return;
        fields->m_riderHidden = false;
        if (m_iconSprite) m_iconSprite->setVisible(true);
        if (m_iconGlow && m_iconGlow->getParent() != m_iconSprite) {
            m_iconGlow->setVisible(fields->m_glowWasVisible);
        }
    }

    void toggleFlyMode(bool enable, bool noEffects) {
        if (!enable) showRider();
        PlayerObject::toggleFlyMode(enable, noEffects);
        if (enable && inHideMode()) hideRider();
    }

    void toggleBirdMode(bool enable, bool noEffects) {
        if (!enable) showRider();
        PlayerObject::toggleBirdMode(enable, noEffects);
        if (enable && inHideMode()) hideRider();
    }

    void update(float dt) {
        PlayerObject::update(dt);
        if (inHideMode()) {
            hideRider();
        } else if (m_fields->m_riderHidden && !m_isShip && !m_isBird) {
            // Safety net: left ship/UFO without a toggle call we saw.
            // Robot & spider hide the cube themselves, so don't re-show it there.
            if (!m_isRobot && !m_isSpider) showRider();
            else m_fields->m_riderHidden = false;
        } else if (m_fields->m_riderHidden) {
            // Still in ship/UFO but the setting was turned off mid-level.
            showRider();
        }
    }
};
