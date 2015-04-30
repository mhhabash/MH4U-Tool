#include <QtSql/QSqlQuery>
#include "damagecalculator.hpp"

float calculate_raw_sharpness(std::string sharpness)
{
    float result;
    if (sharpness == "Purple")
        result = 1.45;
    else if (sharpness == "White")
        result = 1.32;
    else if (sharpness == "Blue")
        result = 1.20;
    else if (sharpness == "Green")
        result = 1.05;
    else if (sharpness == "Yellow")
        result = 1.00;
    else if (sharpness == "Orange")
        result = 0.75;
    else if (sharpness == "Red")
        result = 0.50;
    else
        exit(1);
    return result;
}

float calculate_ele_sharpness(std::string sharpness)
{
    float result;
    if (sharpness == "Purple")
        result = 1.20;
    else if (sharpness == "White")
        result = 1.12;
    else if (sharpness == "Blue")
        result = 1.06;
    else if (sharpness == "Green")
        result = 1.00;
    else if (sharpness == "Yellow")
        result = 0.75;
    else if (sharpness == "Orange")
        result = 0.50;
    else if (sharpness == "Red")
        result = 0.25;
    else
        exit(1);
    return result;
}

float calculate_raw(std::map<std::string, float> modifiers, std::string weapon_type)
{
    float weapon_modifier;
    float result;

    if (weapon_type == "SwitchAxes")
        weapon_modifier = 5.4;
    else if (weapon_type == "Hammers" || weapon_type == "HuntingHorns")
        weapon_modifier = 5.2;
    else if (weapon_type == "GreatSwords")
        weapon_modifier = 4.2;
    else if (weapon_type == "ChargeBlades")
        weapon_modifier = 3.6;
    else if (weapon_type == "LongSwords")
        weapon_modifier = 3.3;
    else if (weapon_type == "InsectGlaives")
        weapon_modifier = 3.1;
    else if (weapon_type == "Lances" || weapon_type == "Gunlances")
        weapon_modifier = 2.3;
    else if (weapon_type == "HeavyBowguns")
        weapon_modifier = 1.5;
    else if (weapon_type == "SwordnShields" || weapon_type == "DualBlades")
        weapon_modifier = 1.4;
    else if (weapon_type == "LightBowguns")
        weapon_modifier = 1.3;
    else if (weapon_type == "Bows")
        weapon_modifier = 1.2;

    result = modifiers["attack"]/weapon_modifier;
    result += modifiers["AuX"];
    result += modifiers["kitchen"];
    result += modifiers["seed"];
    result += modifiers["glove"];
    result += modifiers["powerCharm"] + modifiers["powerTalon"];
    result *= (modifiers["HH"] + modifiers["replay"]);
    result *= modifiers["danger"];
    result *= modifiers["fortify"];

    return result;
}

float calculate_ele(std::map<std::string, float> modifiers)
{
    float result;
    float element_multiplier = 1;

    result = modifiers["specialAttack"]/10;
    result += modifiers["elementAdd"]/10;
    element_multiplier *= modifiers["elementMul"];
    element_multiplier *= (modifiers["elementUp"] + modifiers["HH"]);
    element_multiplier *= modifiers["elementAtkUp"];
    if (element_multiplier > 1.2)
        element_multiplier = 1.2;
    result *= element_multiplier;

    return result;
}

float calculate_affinity(std::map<std::string, float> modifiers)
{
    float result;

    result = modifiers["affinity"];
    result += modifiers["criticalEye"];
    result += modifiers["glove"];
    result += modifiers["criticalDraw"];
    result += modifiers["affinityUp"];
    if (result > 1)
        result = 1;

    return result;
}

float calculate_raw_hitzone(bool weakness_exploit, float raw_hitzone)
{
    float result;

    result = raw_hitzone;
    if (result > 0.45 && weakness_exploit)
        result += 0.05;

    return result;
}

float calculate_ele_hitzone(float ele_hitzone)
{
    float result;

    result = ele_hitzone;

    return result;
}

float calculate_weapon_affinity_multiplier(std::string weapon_type)
{
    float result;

    if (weapon_type == "GreatSwords")
        result = 0.2;
    else if (weapon_type == "SwordnShields" || weapon_type == "DualBlades" || weapon_type == "Bows")
        result = 0.35;
    else
        result = 0.25;

    return result;
}

float calculate_raw_damage(bool weaknessExploit, std::string sharpness, std::string weaponType, float rawHitzone,
                           float motionValue, std::map<std::string, std::map<std::string, float> > all_modifiers)
{
    float rawSharpnessModifier;
    float trueAffinity;
    float trueRaw;
    float trueRawHitzone;
    float trueAffinityMultiplier;

    rawSharpnessModifier = calculate_raw_sharpness(sharpness);
    trueAffinity = calculate_affinity(all_modifiers["affinityModifiers"]);
    trueRaw = calculate_raw(all_modifiers["rawModifiers"], weaponType);
    trueRawHitzone = calculate_raw_hitzone(weaknessExploit, rawHitzone);
    trueAffinityMultiplier = calculate_weapon_affinity_multiplier(weaponType);

    raw_damage = true_raw*(1+trueAffinityMultiplier*true_affinity)*motionValue*true_raw_hitzone*raw_sharpness_modifier;

    return raw_damage;
}

float calculate_ele_damage(bool sharpness, std::map<std::string, std::map<std::string, float> > all_modifiers,
                           float ele_hitzone, QSqlQuery query)
{
    float ele_sharpness_modifier;
    float true_ele;
    float true_ele_hitzone;

    if (sharpness)
        ele_sharpness_modifier = calculate_ele_sharpness("Purple");
    else
        ele_sharpness_modifier = calculate_ele_sharpness("White");

    true_ele = calculate_ele(all_modifiers["eleModifiers"]);
    true_ele_hitzone = calculate_ele_hitzone(ele_hitzone);

    ele_damage = true_ele * true_ele_hitzone * ele_sharpness_modifier;

    return ele_damage;
}

float calculate_total_damage(void)
{
    total_damage = raw_damage + ele_damage;
    return total_damage;
}
