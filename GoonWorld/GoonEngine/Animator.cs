using System.Text.Json;
using GoonEngine.Models;
using GoonEngine.Content;
using GoonEngine.Components;

namespace GoonEngine;
public class Animator<T> where T : GameObject
{
    private static string GetImagePath(string filename) => $"assets/img/{filename}.png";
    public AsepriteDocument BaseDocument => _loadedDocument;
    private AsepriteDocument _loadedDocument;
    public Dictionary<string, Animation<T>> Animations = new();
    public void LoadAnimationFile(string filepath)
    {
        var fullPath = $"assets/img/{filepath}.json";
        string jsonContent = File.ReadAllText(fullPath);
        _loadedDocument = JsonSerializer.Deserialize<AsepriteDocument>(jsonContent);
    }

    public void AddAnimation(Animation<T> animation)
    {
        // animation.Image = Image.LoadImage(GetImagePath(_loadedDocument.meta.image));
        animation.Image = Image.LoadImage(_loadedDocument.meta.image);
        foreach (var transition in _loadedDocument.meta.frameTags)
        {
            if (transition.name == animation.Name)
            {
                {
                    animation.StartFrame = transition.from;
                    animation.EndFrame = transition.to;
                }
            }
        }
        Animations[animation.Name] = animation;
    }

    public AnimatorTransitionArgs<T>? CheckAnimationState(AnimationComponent<T> component)
    {
        if (!Animations.TryGetValue(component.CurrentAnimation, out var animations))
            return null;
        foreach (var transition in animations.Transitions)
        {
            if (transition.TransitionCondition((T)component.Parent))
            {
                return new AnimatorTransitionArgs<T> { Animation = Animations[transition.TransitionAnimationTag], Document = _loadedDocument };
            }
        }
        return null;
    }
}

public struct AnimatorTransitionArgs<T>
{
    public Animation<T> Animation;
    public AsepriteDocument Document;
}