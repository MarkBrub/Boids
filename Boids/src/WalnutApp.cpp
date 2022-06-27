#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Timer.h"
#include <queue>

#include "flightControl.hpp"
#include "util.hpp"

class ExampleLayer : public Walnut::Layer {
public:
	virtual void OnUIRender() override {
		ImGui::Begin("Settings");
		ImGui::Text("Average Render Time: %.2f", m_QueueTotal / std::max((int)m_FrameLengths.size(), 1));
		ImGui::SliderInt("Boid Count", &m_BoidCount, 0, 1000, "%d", 0);
		ImGui::SliderInt("Seperation", &m_SeperationSlider, 1, 50, "%d", 0);
		ImGui::SliderInt("Alignment", &m_AlignmentSlider, 1, 500, "%d", 0);
		ImGui::SliderInt("Cohesion", &m_CohesionSlider, 1, 550, "%d", 0);
		ImGui::SliderInt("Max Speed", &m_MaxSpeed, 0, 10, "%d", 0);
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");

		m_ViewportWidth = ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = ImGui::GetContentRegionAvail().y;

		if (m_Image) {
			ImGui::Image(m_Image->GetDescriptorSet(), { (float)m_Image->GetWidth(), (float)m_Image->GetHeight() });
		}

		Render();

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void Render() {
		Walnut::Timer timer;

		if (!m_Image || m_ViewportWidth != m_Image->GetWidth() || m_ViewportHeight != m_Image->GetHeight()) {
			m_Image = std::make_shared<Walnut::Image>(m_ViewportWidth, m_ViewportHeight, Walnut::ImageFormat::RGBA);

			delete[] m_ImageData;
			m_ImageData = new uint32_t[m_ViewportWidth * m_ViewportHeight];

			m_Controller.Resize(m_ViewportWidth, m_ViewportHeight);
		}

		if (m_BoidCount != m_Controller.GetBoidCount()) {
			m_Controller.ChangePopulationSize(m_BoidCount);
		}
		if (m_SeperationSlider != m_Controller.seperation) {
			m_Controller.seperation = m_SeperationSlider;
		}
		if (m_AlignmentSlider != m_Controller.alignment) {
			m_Controller.alignment = m_AlignmentSlider;
		}
		if (m_CohesionSlider != m_Controller.cohesion) {
			m_Controller.cohesion = m_CohesionSlider;
		}
		if (m_MaxSpeed != m_Controller.maxSpeed) {
			m_Controller.maxSpeed = m_MaxSpeed;
		}

		m_Controller.AdvanceFrame();

		for (unsigned int x = 0; x < m_ViewportWidth * m_ViewportHeight; x++) {
			m_ImageData[x] = Color::toUint32(44, 57, 75);
		}

		m_Controller.Render(m_ImageData);

		m_Image->SetData(m_ImageData);

		m_RenderTime = timer.ElapsedMillis();

		m_FrameLengths.emplace_back(m_RenderTime);
		m_QueueTotal += m_RenderTime;

		while (m_QueueTotal > m_TrackedTime) {
			m_QueueTotal -= m_FrameLengths.front();
			m_FrameLengths.pop_front();
		}
	}

private:
	std::shared_ptr<Walnut::Image> m_Image;
	uint32_t* m_ImageData = nullptr;
	uint32_t m_ViewportWidth = 0;
	uint32_t m_ViewportHeight = 0;

	// Variables for getting average framerate
	std::deque<float> m_FrameLengths;
	const float m_TrackedTime = 1500; // number of milliseconds to average framerate over
	float m_RenderTime = 0;
	float m_QueueTotal = 0;
	FlightControl m_Controller;
	int m_BoidCount = 1000;
	int m_SeperationSlider = 16;
	int m_AlignmentSlider = 250;
	int m_CohesionSlider = 350;
	int m_MaxSpeed = 3;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv) {
	Walnut::ApplicationSpecification spec;
	spec.Name = "Boids";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	/*
	app->SetMenubarCallback([app]() {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Exit")) {
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	*/
	return app;
}