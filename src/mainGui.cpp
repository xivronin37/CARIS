#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <chrono>
#include <thread>
#include "implot.h"
#include "init.h"
#include "economy.h"
#include "gov.h"
#include "sim.h"
#include "industry.h"
#include "invest.h"


int main() {
    Init init;
    Economy econ;
    Government gov(init, econ);
    Industries ind(econ, gov);
    Investor inv(econ, gov, ind);
    Simulation sim(econ, gov, ind, inv);

    double timeAccumulator = 0.0;
    double secondsPerTick = 0.5;
    bool playing = false;
    std::string selectedChart = "unselected";
    std::vector<double> gdpHistory, tickHistory, portfolioHistory;
    std::vector<double> agrPriceHistory, manPriceHistory, conPriceHistory, enePriceHistory, serPriceHistory;
    const char* targets[] = { "AGR", "MAN", "CON", "ENE", "SER", "IDX", "BND" };
    int selectedTarget = 0;
    double buyAmount = 0.0;
    float speedVal = 1.0f;

    glfwInit();
    GLFWwindow* window = glfwCreateWindow(1280, 720, "CARIS", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    ImGui::StyleColorsDark();  // start from the built-in dark theme as a base
    ImGuiStyle& style = ImGui::GetStyle();

    // Rounding — softer edges instead of sharp rectangles
    style.WindowRounding = 8.0f;
    style.FrameRounding = 6.0f;
    style.GrabRounding = 6.0f;
    style.PopupRounding = 6.0f;
    style.ScrollbarRounding = 8.0f;

    // Spacing — a bit more breathing room than the cramped default
    style.WindowPadding = ImVec2(12, 12);
    style.FramePadding = ImVec2(8, 6);
    style.ItemSpacing = ImVec2(10, 8);

    // A cohesive accent color (a muted blue) instead of ImGui's stock bright blue
    ImVec4 accent = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(accent.x, accent.y, accent.z, 0.60f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(accent.x, accent.y, accent.z, 0.80f);
    style.Colors[ImGuiCol_ButtonActive] = accent;
    style.Colors[ImGuiCol_SliderGrab] = accent;
    style.Colors[ImGuiCol_SliderGrabActive] = accent;
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(accent.x, accent.y, accent.z, 0.25f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(accent.x * 0.5f, accent.y * 0.5f, accent.z * 0.5f, 1.00f);

    // Slightly darker window background than stock, for contrast
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.10f, 1.00f);

    glClearColor(0.133f, 0.203f, 0.164f, 1.0f);

    auto normalize = [](const std::vector<double>& history) {
        std::vector<double> result;
        if (history.empty()) return result;
        double start = history.front();
        for (double v : history) {
            result.push_back((v / start) * 100.0);
        }
        return result;
    };

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Settings");
        if (ImGui::Button("Play/Pause")) {
            playing = !playing;
        }

        ImGui::Text(playing ? "Status: Playing" : "Status: Paused");
        if (ImGui::SliderFloat("Speed", &speedVal, 0.1f, 4.0f)) {
            secondsPerTick = 1 / speedVal;
        }

        if (playing) {
            timeAccumulator += ImGui::GetIO().DeltaTime;
            while (timeAccumulator >= secondsPerTick) {
                sim.tick();
                gdpHistory.push_back(econ.gdp);
                tickHistory.push_back(sim.tickNumber);
                portfolioHistory.push_back(inv.totalValue());
                agrPriceHistory.push_back(ind.agriculture.price);
                manPriceHistory.push_back(ind.manufacturing.price);
                conPriceHistory.push_back(ind.construction.price);
                enePriceHistory.push_back(ind.energy.price);
                serPriceHistory.push_back(ind.services.price);
                timeAccumulator -= secondsPerTick;
            }
        }

        ImGui::Text("Tick: %d", sim.tickNumber);
        ImGui::End();

        ImGui::SetNextWindowSize(ImVec2(500, 400));
        ImGui::Begin("Charts");
        if (ImGui::Button("GDP")) {
            selectedChart = "GDP";
        }
        ImGui::SameLine();
        if (ImGui::Button("Portfolio")) {
            selectedChart = "Portfolio";
        }
        ImGui::SameLine();
        if (ImGui::Button("Industries (Raw)")) {
            selectedChart = "IndustriesRaw";
        }
        ImGui::SameLine();
        if (ImGui::Button("Industries (Indexed)")) {
            selectedChart = "IndustriesIndexed";
        }

        if (selectedChart == "GDP") {
            if (ImPlot::BeginPlot("GDP")) {
                ImPlot::SetupAxes("Tick", "GDP", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
                ImPlot::PlotLine("GDP", tickHistory.data(), gdpHistory.data(), gdpHistory.size());
                ImPlot::EndPlot();
            }
        } else if (selectedChart == "Portfolio") {
            if (ImPlot::BeginPlot("Portfolio")) {
                ImPlot::SetupAxes("Tick", "Value", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
                ImPlot::PlotLine("Value", tickHistory.data(), portfolioHistory.data(), portfolioHistory.size());
                ImPlot::EndPlot();
            }
        } else if (selectedChart == "IndustriesRaw") {
            if (ImPlot::BeginPlot("Industry Prices")) {
                ImPlot::SetupAxes("Tick", "Price", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
                ImPlot::PlotLine("Agriculture", tickHistory.data(), agrPriceHistory.data(), agrPriceHistory.size());
                ImPlot::PlotLine("Manufacturing", tickHistory.data(), manPriceHistory.data(), manPriceHistory.size());
                ImPlot::PlotLine("Construction", tickHistory.data(), conPriceHistory.data(), conPriceHistory.size());
                ImPlot::PlotLine("Energy", tickHistory.data(), enePriceHistory.data(), enePriceHistory.size());
                ImPlot::PlotLine("Services", tickHistory.data(), serPriceHistory.data(), serPriceHistory.size());
                ImPlot::EndPlot();
            }
        } else if (selectedChart == "IndustriesIndexed") {
            if (ImPlot::BeginPlot("Industry Prices (Indexed at 100)")) {
                auto agrN = normalize(agrPriceHistory);
                auto manN = normalize(manPriceHistory);
                auto conN = normalize(conPriceHistory);
                auto eneN = normalize(enePriceHistory);
                auto serN = normalize(serPriceHistory);
                ImPlot::SetupAxes("Tick", "Index (start = 100)", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
                ImPlot::PlotLine("Agriculture", tickHistory.data(), agrN.data(), agrN.size());
                ImPlot::PlotLine("Manufacturing", tickHistory.data(), manN.data(), manN.size());
                ImPlot::PlotLine("Construction", tickHistory.data(), conN.data(), conN.size());
                ImPlot::PlotLine("Energy", tickHistory.data(), eneN.data(), eneN.size());
                ImPlot::PlotLine("Services", tickHistory.data(), serN.data(), serN.size());
                ImPlot::EndPlot();
            }
        }
        ImGui::End();

        ImGui::SetNextWindowSize(ImVec2(400, 300));
        ImGui::Begin("Economy");
        ImGui::Text("Population: %s", formatNumber(econ.population).c_str());
        ImGui::Text("Labor Participation: %s %", formatNumber(econ.laborParticipation * 100).c_str());
        ImGui::Text("Productivity: %s", formatNumber(econ.population).c_str());
        ImGui::Text("Consumption: %s %", formatNumber(econ.laborParticipation).c_str());
        ImGui::Text("Investment: %s", formatNumber(econ.population).c_str());
        ImGui::Text("GDP: %s", formatNumber(econ.gdp).c_str());
        ImGui::Text("Inflation: %s %", formatNumber(econ.inflation * 100).c_str());
        ImGui::End();

        ImGui::SetNextWindowSize(ImVec2(400, 300));
        ImGui::Begin("Government");
        ImGui::Text("Tax Rate: %s%", formatNumber(gov.taxRate*100).c_str());
        ImGui::Text("Government Spending: $%s", formatNumber(gov.governmentSpending).c_str());
        ImGui::Text("Debt: $%s", formatNumber(gov.debt).c_str());
        ImGui::Text("Debt Ratio: %s", formatNumber(gov.debtRatio).c_str());
        ImGui::End();

        ImGui::SetNextWindowSize(ImVec2(400, 300));
        ImGui::Begin("Investor");
        ImGui::Text("Cash: %s", formatNumber(inv.cash).c_str());
        
        ImGui::Combo("Target", &selectedTarget, targets, IM_ARRAYSIZE(targets));
        if (ImGui::Button("Buy")) {
            inv.buy(targets[selectedTarget], buyAmount);
        }
        ImGui::SameLine();
        if (ImGui::Button("Sell")) {
            inv.sell(targets[selectedTarget], buyAmount);
        }
        ImGui::InputDouble("Amount (in cash)", &buyAmount);
        ImGui::End();




        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}