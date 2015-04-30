#ifndef DAMAGECALCULATOR_HPP
#define DAMAGECALCULATOR_HPP
#include <map>
#include <string>

float raw_damage = 0.0;
float ele_damage = 0.0;
float total_damage = 0.0;

float calculate_raw_sharpness(std::string sharpness);

float calculate_ele_sharpness(std::string sharpness);

float calculate_raw(std::map<std::string, float> modifiers, std::string weapon_type);

float calculate_ele(std::map<std::string, float> modifiers);

float calculate_affinity(std::map<std::string, float> modifiers);

float calculate_raw_hitzone(std::map<std::string, float> modifiers, float raw_hitzone);

float calculate_ele_hitzone(float ele_hitzone);

float calculate_weapon_affinity_multiplier(std::string weapon_type);

float calculate_raw_damage(bool sharpness, std::map<std::string, std::map<std::string, float> > all_modifiers,
                           std::string weapon_type, float raw_hitzone, float ele_hitzone);

float calculate_ele_damage(bool sharpness, std::map<std::string, std::map<std::string, float> > all_modifiers,
                           std::string weapon_type, float raw_hitzone, float ele_hitzone);

float calculate_total_damage(void);

#endif // DAMAGECALCULATOR_HPP
