#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <fstream>

TEST(ConfigTest, ParseDefault) {
    std::ifstream ifs("resources/default_config.json");
    ASSERT_TRUE(ifs.good());
    auto j = nlohmann::json::parse(ifs);
    ASSERT_TRUE(j.contains("listen_port"));
    EXPECT_EQ(j["listen_port"].get<int>(), 8080);
}
