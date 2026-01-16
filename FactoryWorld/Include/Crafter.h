#pragma once
#include "SDL.h"
#include "GameObject.h"
#include "Item.h"
#include <string>
#include <array>
#include <vector>

class Crafter :public GameObject
{
public:
	Crafter(Orientation orientation = Orientation::NORTH, std::string recipeId = "iron_ingot");
	~Crafter();

	void Start() override;
	void Update(uint32_t now) override;
	void Render() override;

	void StopCrafter();

	void ChangeRecipe(const std::string& newId);
	void ChangeNextRecipe();

	// Input
	bool CanAcceptInput() const;
	bool InsertInput(const Item& item);

	// Output
	bool HasOutput() const;

	std::string GetRecipeId() { return RecipeId; }

private:

	void TryStartCraft();
	void FinishCraft();

	void RebuildRecipeCache();
	bool CanAcceptInputType(ItemType t) const;
	bool HasAllInputsForRecipe() const;
	void ConsumeType(ItemType t, int count);


	std::string RecipeId;

	std::vector<Item*> mInputSlots;

	LTexture* itemTexture = nullptr;

	Item* mOutputItem = nullptr;

	std::array<int, ITEMTYPE_COUNT> mNeed{};
	std::array<int, ITEMTYPE_COUNT> mHave{};

	Uint32 craftingMaxTime = 0;
	Uint32 craftingCurrentTime = 0;

	bool isCrafting = false;
	bool isOn = false;
};