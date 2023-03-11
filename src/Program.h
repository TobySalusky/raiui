//
// Created by Tobiathan on 12/10/22.
//

#ifndef MATHVIEW_PROGRAM_H
#define MATHVIEW_PROGRAM_H

class Program {
public:
    void Run();

private:
    int SCREEN_WIDTH = 1920 / 2;
    int SCREEN_HEIGHT = 1080 / 2;

    void Update(float deltaTime);

    void Render();

    void LayerUI();
};

#endif //MATHVIEW_PROGRAM_H
