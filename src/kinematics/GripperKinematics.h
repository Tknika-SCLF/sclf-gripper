#pragma once
/**
 * GripperKinematics.h — Pintzaren Zinematika Modulua
 *
 * Motorraren angelua (rad) ↔ Hatzen irekidura (mm) bihurtzeko.
 * Robotiq 2F-85/2F-140 pintzetan oinarrituta, baina parametrizatua
 * edozein pintza mekanikarako erraz egokitzeko.
 *
 * ⚠️  FASE 3.2 — Placeholder balioak Robotiq 2F-85 oinarrituz
 *     Mekanikoak diseinu zehatza amaitzen dutenean, aldatu
 *     GripperConfig balioak zuen pintzara egokitzeko.
 */

#include <Arduino.h>

// ─── Pintzaren Konfigurazio Parametroak ──────────────────────────────────────
struct GripperConfig {
    // Mekanika
    float stroke_mm;   // Irekidura maximoa (mm). 2F-85=85, 2F-140=140
    float gear_ratio;  // Engranaje erlazioa (motor bira / usillo bira)
    float lead_mm;     // Usilloaren urratsa (mm/bira). Bira batean zenbat mm mugitzen den

    // Muga fisikoak (motor angeluetan, radianetan)
    float min_angle_rad;  // Guztiz irekita dagoeneko angelua (rad)
    float max_angle_rad;  // Guztiz itxita dagoeneko angelua (rad)

    // Segurtasun mugak
    float max_force_N;  // Helmen indar maximoa (N)
};

// ─── Aurredefinitutako Konfigurazioak ────────────────────────────────────────
namespace GripperPresets {

// Robotiq 2F-85: 85mm stroke, lead screw mekanismoa
constexpr GripperConfig ROBOTIQ_2F85 = {
    .stroke_mm = 85.0f,
    .gear_ratio = 1.0f,     // Zuzeneko transmisioa (placeholder)
    .lead_mm = 1.0f,        // 1mm per motor turn (placeholder — egokitu)
    .min_angle_rad = 0.0f,  // Irekita = 0 rad
    .max_angle_rad = 0.0f,  // Kalkulatua init()-en
    .max_force_N = 235.0f,  // Robotiq 2F-85 max grip force
};

// Robotiq 2F-140: 140mm stroke, mekanika berdina hatz luzeagoekin
constexpr GripperConfig ROBOTIQ_2F140 = {
    .stroke_mm = 140.0f,
    .gear_ratio = 1.0f,
    .lead_mm = 1.0f,
    .min_angle_rad = 0.0f,
    .max_angle_rad = 0.0f,
    .max_force_N = 249.0f,
};
}  // namespace GripperPresets

// ─── Zinematika Klasea ───────────────────────────────────────────────────────
class GripperKinematics {
   public:
    /**
     * Hasieratu zinematika konfigurazio batekin.
     * max_angle_rad automatikoki kalkulatzen du stroke eta lead-etik.
     */
    void begin(const GripperConfig& cfg);

    /**
     * Motorraren angelua (rad) → Hatzen irekidura (mm).
     * @param angle_rad  Motorraren angelua radianetan
     * @return Irekidura milimetrotan [0, stroke_mm]
     */
    float angleToMm(float angle_rad) const;

    /**
     * Hatzen irekidura (mm) → Motorraren angelua (rad).
     * @param mm  Nahi den irekidura milimetrotan
     * @return Motorraren angelu helburua radianetan
     */
    float mmToAngle(float mm) const;

    /**
     * Irekidura portzentajean (0-100%) → Motorraren angelua (rad).
     * @param pct  0.0 = guztiz irekita, 100.0 = guztiz itxita
     */
    float percentToAngle(float pct) const;

    /**
     * Motorraren angelua → Irekidura portzentajean (0-100%).
     */
    float angleToPercent(float angle_rad) const;

    /**
     * Egiaztatu angelu bat muga fisikoen barruan dagoen.
     * @return Angelua mugatuta [min_angle, max_angle]
     */
    float clampAngle(float angle_rad) const;

    /**
     * Egiaztatu irekidura mm-tan muga fisikoen barruan dagoen.
     * @return Irekidura mugatuta [0, stroke_mm]
     */
    float clampMm(float mm) const;

    // Getterrak
    float getStrokeMm() const { return _cfg.stroke_mm; }
    float getMinAngle() const { return _cfg.min_angle_rad; }
    float getMaxAngle() const { return _cfg.max_angle_rad; }
    float getMaxForce() const { return _cfg.max_force_N; }
    const GripperConfig& getConfig() const { return _cfg; }

    /**
     * Debug: Inprimatu konfigurazio guztia Serial-era.
     */
    void printConfig() const;

   private:
    GripperConfig _cfg;
    float _radPerMm;  // Kalkulatutako faktorea: rad/mm
};
