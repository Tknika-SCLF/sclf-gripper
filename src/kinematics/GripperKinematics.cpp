/**
 * GripperKinematics.cpp — Pintzaren Zinematika Inplementazioa
 *
 * Motorraren biraketa lineala ↔ Hatzen irekidura (mm)
 * Lead screw mekanismo sinplea:
 *   mm = (angle_rad / (2π * gear_ratio)) * lead_mm
 *
 * ⚠️  FASE 3.2
 */

#include "kinematics/GripperKinematics.h"

#ifndef TWO_PI
#define TWO_PI 6.28318530718f
#endif

void GripperKinematics::begin(const GripperConfig& cfg) {
    _cfg = cfg;

    // Kalkulatu rad-per-mm faktorea: zenbat radian behar diren mm bat mugitzeko
    // Lead screw: bira bat (2π rad * gear_ratio) = lead_mm milimetro
    // Beraz: 1 mm = (2π * gear_ratio) / lead_mm radian
    if (_cfg.lead_mm > 0.0f) {
        _radPerMm = (TWO_PI * _cfg.gear_ratio) / _cfg.lead_mm;
    } else {
        _radPerMm = 1.0f;  // Fallback segurua
    }

    // max_angle automatikoki kalkulatu stroke-tik
    if (_cfg.max_angle_rad == 0.0f) {
        _cfg.max_angle_rad = _cfg.stroke_mm * _radPerMm;
    }
}

float GripperKinematics::angleToMm(float angle_rad) const {
    // Angelu mugatu barrutian
    float clamped = clampAngle(angle_rad);
    // Bihurtu mm-tara
    float mm = clamped / _radPerMm;
    return clampMm(mm);
}

float GripperKinematics::mmToAngle(float mm) const {
    float clamped_mm = clampMm(mm);
    return clamped_mm * _radPerMm;
}

float GripperKinematics::percentToAngle(float pct) const {
    // 0% = irekita (min_angle), 100% = itxita (max_angle)
    float p = constrain(pct, 0.0f, 100.0f);
    float range = _cfg.max_angle_rad - _cfg.min_angle_rad;
    return _cfg.min_angle_rad + (p / 100.0f) * range;
}

float GripperKinematics::angleToPercent(float angle_rad) const {
    float clamped = clampAngle(angle_rad);
    float range = _cfg.max_angle_rad - _cfg.min_angle_rad;
    if (range < 0.001f)
        return 0.0f;
    return ((clamped - _cfg.min_angle_rad) / range) * 100.0f;
}

float GripperKinematics::clampAngle(float angle_rad) const {
    if (angle_rad < _cfg.min_angle_rad)
        return _cfg.min_angle_rad;
    if (angle_rad > _cfg.max_angle_rad)
        return _cfg.max_angle_rad;
    return angle_rad;
}

float GripperKinematics::clampMm(float mm) const {
    if (mm < 0.0f)
        return 0.0f;
    if (mm > _cfg.stroke_mm)
        return _cfg.stroke_mm;
    return mm;
}

void GripperKinematics::printConfig() const {
    Serial.println("─── Gripper Kinematics Config ───");
    Serial.print("  Stroke:       ");
    Serial.print(_cfg.stroke_mm);
    Serial.println(" mm");
    Serial.print("  Gear ratio:   ");
    Serial.println(_cfg.gear_ratio);
    Serial.print("  Lead:         ");
    Serial.print(_cfg.lead_mm);
    Serial.println(" mm/rev");
    Serial.print("  Min angle:    ");
    Serial.print(_cfg.min_angle_rad);
    Serial.println(" rad");
    Serial.print("  Max angle:    ");
    Serial.print(_cfg.max_angle_rad);
    Serial.println(" rad");
    Serial.print("  Max force:    ");
    Serial.print(_cfg.max_force_N);
    Serial.println(" N");
    Serial.print("  rad/mm:       ");
    Serial.println(_radPerMm);
    Serial.println("─────────────────────────────────");
}
