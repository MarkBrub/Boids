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
		ImGui::SliderInt("Boid Count", &m_BoidCount, 0, 1500, "%d", 0);
		ImGui::SliderInt("Seperation", &m_SeperationSlider, 0, m_MaxSeperation, "%d", 0);
		ImGui::SliderInt("Alignment", &m_AlignmentSlider, 0, m_MaxAlignment, "%d", 0);
		ImGui::SliderInt("Cohesion", &m_CohesionSlider, 0, m_MaxCohesion, "%d", 0);
		ImGui::SliderInt("Max Speed", &m_MaxSpeed, 0, 20, "%d", 0);
		ImGui::SliderInt("Vision Radius", &m_VisionRadius, 50, 150, "%d", 0);
		ImGui::SliderInt("Avoidance Radius", &m_AvoidanceRadius, 10, 30, "%d", 0);
		ImGui::SliderInt("Field of View", &m_FieldOfView, 0, 360, "%d", 0);
		ImGui::SliderFloat("Scale", &m_Scale, 0, 3, "%.2f", 0);
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

		// If the viewport is not initilized or has changed size then reallocte buffer and update FlightController size
		if (!m_Image || m_ViewportWidth != m_Image->GetWidth() || m_ViewportHeight != m_Image->GetHeight()) {
			m_Image = std::make_shared<Walnut::Image>(m_ViewportWidth, m_ViewportHeight, Walnut::ImageFormat::RGBA);

			delete[] m_ImageData;
			m_ImageData = new uint32_t[m_ViewportWidth * m_ViewportHeight];

			m_Controller.Resize(m_ViewportWidth, m_ViewportHeight);
		}

		// If any slider value has changed then update value in FlightController
		if (m_BoidCount != m_Controller.GetBoidCount()) {
			m_Controller.ChangePopulationSize(m_BoidCount);
		}
		if (m_SeperationSlider != m_Controller.seperation) {
			m_Controller.seperation = m_MaxSeperation - m_SeperationSlider + 1;
		}
		if (m_AlignmentSlider != m_Controller.alignment) {
			m_Controller.alignment = m_MaxAlignment - m_AlignmentSlider + 1;
		}
		if (m_CohesionSlider != m_Controller.cohesion) {
			m_Controller.cohesion = m_MaxCohesion - m_CohesionSlider + 1;
		}
		if (m_MaxSpeed != m_Controller.maxSpeed) {
			m_Controller.maxSpeed = m_MaxSpeed;
		}
		if (m_VisionRadius != m_Controller.visionRadius) {
			m_Controller.visionRadius = m_VisionRadius;
		}
		if (m_AvoidanceRadius != m_Controller.avoidanceRadius) {
			m_Controller.avoidanceRadius = m_AvoidanceRadius;
		}
		if (m_FieldOfView != m_Controller.fieldOfView) {
			m_Controller.fieldOfView = m_FieldOfView;
		}
		if (m_Scale != m_Controller.scale) {
			// Rounds down to the nearest .25
			m_Scale = (float)((int)(m_Scale * 4)) / 4;
			m_Controller.scale = m_Scale;
		}

		// Perform all calculations
		m_Controller.AdvanceFrame();

		// Reset the background color
		for (unsigned int x = 0; x < m_ViewportWidth * m_ViewportHeight; x++) {
			m_ImageData[x] = m_backgroundColor;
		}

		// Draw boids on top of background
		m_Controller.Render(m_ImageData);

		// Set buffer to be rendered
		m_Image->SetData(m_ImageData);

		// Keep track of render times for rolling average
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
	const float m_TrackedTime = 500; // number of milliseconds to average framerate over
	float m_RenderTime = 0;
	float m_QueueTotal = 0;

	FlightControl m_Controller;
	int m_BoidCount = 1000;
	int m_MaxSeperation = 30;
	int m_MaxAlignment = 500;
	int m_MaxCohesion = 500;
	int m_SeperationSlider = 15;
	int m_AlignmentSlider = 250;
	int m_CohesionSlider = 250;
	int m_MaxSpeed = 5;
	int m_VisionRadius = 100;
	int m_AvoidanceRadius = 20;
	int m_FieldOfView = 270;
	float m_Scale = 1;

	uint32_t m_backgroundColor = Util::toUint32({ 44, 57, 75 });
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