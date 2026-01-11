#pragma once
#include <vector>
#include <string>
#include "Singleton.h"
#include "Recipe.h"

class RecipeManager : public Singleton<RecipeManager>
{
    friend class Singleton<RecipeManager>;

private:
    RecipeManager() {}

    std::vector<Recipe> mRecipes;

public:
    void InitDefaults();

    const Recipe* GetRecipeById(const std::string& id) const;
    const std::vector<Recipe>& GetAll() const { return mRecipes; }

    const Recipe* GetNextRecipeById(const std::string& id) const;

    bool AddRecipe(const Recipe& newRecipe);
};
