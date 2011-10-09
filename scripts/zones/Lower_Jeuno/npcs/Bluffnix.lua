-----------------------------------
-- Area: Lower Jeuno
-- NPC: Bluffnix
-- Starts and Finishes Quests: Gobbiebags I-X
-----------------------------------

require("scripts/globals/titles");
require("scripts/globals/settings");
require("scripts/globals/quests");
require("scripts/zones/Lower_Jeuno/TextIDs");

-----------------------------------
-- onTrade Action
-----------------------------------

function onTrade(player,npc,trade)

	count = trade:getItemCount();
	gil = trade:getGil();
	inventorySize = player:getContainerSize(0);
    TheGobbieBag = gobQuest(player,inventorySize);

    if (count == 4 and gil == 0 and player:getQuestStatus(JEUNO,TheGobbieBag[1]) == 1) then
		if (trade:hasItemQty(TheGobbieBag[3],1) and trade:hasItemQty(TheGobbieBag[4],1) and trade:hasItemQty(TheGobbieBag[5],1) and trade:hasItemQty(TheGobbieBag[6],1)) then
			player:startEvent(0x0049, inventorySize+1);
		end
	end
end;

---------------------------------------------
-- Current Quest, Required Fame and Items
---------------------------------------------
function gobQuest(player,bagSize)
	currentQuest = {};
	switch (bagSize) : caseof {
		[30] = function (x) currentQuest = {27,3,0848,0652,0826,0788}; end, --Gobbiebag I, Dhalmel Leather, Steel Ingot, Linen Cloth, Peridot
		[35] = function (x) currentQuest = {28,4,0851,0653,0827,0798}; end, --Gobbiebag II, Ram Leather, Mythril Ingot, Wool Cloth, Turquoise
		[40] = function (x) currentQuest = {29,5,0855,0745,0828,0797}; end, --Gobbiebag III, Tiger Leather, Gold Ingot, Velvet Cloth, Painite
		[45] = function (x) currentQuest = {30,5,0931,0654,0829,0808}; end, --Gobbiebag IV, Cermet Chunk, Darksteel Ingot, Silk Cloth, Goshenite
		[50] = function (x) currentQuest = {74,6,1637,1635,1636,1634}; end, --Gobbiebag V, Bugard Leather, Paktong Ingot, Moblinweave, Rhodonite
		[55] = function (x) currentQuest = {75,6,1741,1738,1739,1740}; end, --Gobbiebag VI, HQ Eft Skin, Shakudo Ingot, Balloon Cloth, Iolite
		[60] = function (x) currentQuest = {93,7,2530,0655,0830,0812}; end, --Gobbiebag VII, Lynx Leather, Adaman Ingot, Rainbow Cloth, Deathstone
		[65] = function (x) currentQuest = {94,7,2529,2536,2537,0813}; end, --Gobbiebag VIII, Smilodon Leather, Electrum Ingot, Square of Cilice, Angelstone
		[70] = function (x) currentQuest = {123,8,2538,0747,2704,2743}; end, --Gobbiebag IX, Peiste Leather, Orichalcum Ingot, Oil-Soaked Cloth, Oxblood Orb
		[75] = function (x) currentQuest = {124,9,1459,1711,2705,2744}; end, --Gobbiebag X, Griffon Leather, Molybdenum Ingot, Foulard, Angelskin Orb
	default = function (x) end, }
	return currentQuest;
end;

----------------------------------
-- onTrigger Action
-----------------------------------

function onTrigger(player,npc)

    if (player:getContainerSize(0) < 80) then
        pFame = player:getFameLevel(5);
        inventorySize = player:getContainerSize(0);
	    TheGobbieBag = gobQuest(player,inventorySize);
        questStatus = player:getQuestStatus(JEUNO,TheGobbieBag[1]);

        offer = 0;
        if (pFame >= TheGobbieBag[2]) then
            offer = 1;
        end

        player:startEvent(0x002b,inventorySize+1,questStatus,offer);
	else
		player:startEvent(0x002b,81); -- You're bag's bigger than any gobbie bag I've ever seen...;
	end
end;

-----------------------------------
-- onEventUpdate
-----------------------------------

function onEventUpdate(player,csid,option)
--printf("CSID: %u",csid);
--printf("RESULT: %u",option);
end;

-----------------------------------
-- onEventFinish
-----------------------------------

function onEventFinish(player,csid,option)
--printf("CSID: %u",csid);
--printf("RESULT: %u",option);

	TheGobbieBag = gobQuest(player,player:getContainerSize(0));

	if (csid == 0x002b and option == 0) then
		if (player:getQuestStatus(JEUNO,TheGobbieBag[1]) == 0) then
			player:addQuest(JEUNO,TheGobbieBag[1]);
		end
	elseif (csid == 0x0049) then
		player:completeQuest(JEUNO,TheGobbieBag[1]);
		player:addFame(SAN_D_ORIA,20*SAN_FAME); --I'm still not sure what the fame scale is here!
		player:addFame(BASTOK,20*BAS_FAME);
		player:addFame(WINDURST,20*WIN_FAME);
		player:increaseContainerSize(0,5);
		player:increaseContainerSize(5,5);
		player:tradeComplete();

		if (gobbieBag == 5) then
			player:setTitle(GREEDALOX);
		elseif (gobbieBag == 10) then
			player:setTitle(GRAND_GREEDALOX);
		end
	end
end;
