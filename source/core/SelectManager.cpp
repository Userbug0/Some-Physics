#include "../../include/core/SelectManager.hpp"


SelectManager::SelectManager(sf::RenderWindow* window)
    : selected_(nullptr), rightButton_(false), leftButton_(false)
{

}


void SelectManager::Deselect()
{
    if (selected_ != nullptr)
    {
        selected_->OnDeselect(SelectParam);

        selected_ = nullptr;
        rightButton_ = false;
        leftButton_ = false;
    }
}


Selectable* SelectManager::selectObject(const Vector2d& mpos)
{
    for (auto& object : objects_)
    {
        if (object->isInside(mpos))
            return object;
    }
    return nullptr;
}


void SelectManager::onMouseButtonPressed(const Vector2d& mpos, const sf::Mouse::Button& button)
{
    Selectable* candidate = selectObject(mpos);

    if (candidate != nullptr) 
    {
        if (selected_ != candidate || selected_ == nullptr) 
        {
            if (selected_ != nullptr)
                selected_->OnDeselect(SelectParam);
            selected_ = candidate;
            selected_->OnSelect(SelectParam);
        }
        if (button == sf::Mouse::Right)
            onRightButtonPressed(mpos);
        else if (button == sf::Mouse::Left)
            onLeftButtonPressed(mpos);
    }
}


void SelectManager::onMouseMoved(const Vector2d& mpos)
{
    if (selected_ != nullptr) 
    {
        if (rightButton_)
            selected_->OnRightMouseMove(mpos);
        else if (leftButton_)
            selected_->OnLeftMouseMove(mpos);
    }
}


sf::Event::EventType SelectManager::HandleMousePos(sf::RenderWindow* window, const sf::Event& event)
{
    if (event.type == sf::Event::MouseMoved) 
    {
        Vector2d mpos = sf::Mouse::getPosition(*window);
        onMouseMoved(mpos);
    }
    else if (event.type == sf::Event::MouseButtonReleased) 
    {
        Vector2d mpos = sf::Mouse::getPosition(*window);
        onMouseButtonReleased(mpos);
    }
    else if (event.type == sf::Event::MouseButtonPressed) 
    {
        Vector2d mpos = sf::Mouse::getPosition(*window);
        onMouseButtonPressed(mpos, event.mouseButton.button);
    }

    return event.type;
}