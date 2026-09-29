#pragma once

#include <optional>
#include <string>

std::optional<int> LoadAirportElevation(const std::string& icao, const std::string& dllPath);
