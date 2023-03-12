//
// Created by Tobiathan on 12/10/22.
//

#pragma once

class Program {
public:
    void Run();

private:
    int SCREEN_WIDTH = 1920 / 2;
    int SCREEN_HEIGHT = 1080 / 2;

    void Update(float deltaTime);

    void Render();

    void UI();

    void LayerUI();
    void BottomBarUI();

    void ContentStripUI();

    void LeftPanelUI();
    void RightPanelUI();
};
