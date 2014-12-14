package main;

import buffs.Buff;
import buffs.Buff.BuffValues;
import items.ConsumableItem;
import items.Equipment;
import items.Equipment.equipmentTypes;

/*
 * Class to automatically generate Actors, items and more.
 * Before I go farther, I want to make sure that we have all constructors how we want them.
 */
public class Macros {

	public static Actor actor (String actorType, Coordinate location) {
		String actorImage = "res/Default.png";
		int actorRender = 0;
		// Variables needed to create an actor.
		Actor generatedActor = null;

		if (actorType.equalsIgnoreCase("grass")) {
			actorImage = "res/grass.png";
			// Grass is a background actor, and therefore is rendered first.

		} else if (actorType.equalsIgnoreCase("dirt") ) {	
			actorImage = "res/dirt.png";
			// Dirt is a background actor.
			
		} else if (actorType.equalsIgnoreCase("sand") ) {	
			actorImage = "res/sand.png";
			// Sand is a background actor
			
		} else if (actorType.equalsIgnoreCase("water") ) {
			actorImage = "res/water.png";
			// TODO when we have collision channels, this should stop Characters
			// Water is a background actor that collides
			
		} else if (actorType.equalsIgnoreCase("tree")) {
			generatedActor = new Actor(1, "res/tree.png", location);
			// Trees have a render order of 1. Currently.
			// Trees also need to be solid.
			
		} else {
			
		}

		if (generatedActor == null) {
			generatedActor = new Actor(actorRender, actorImage, location);
		}
		
		return generatedActor;
	}
	
	public static ConsumableItem consumable (String itemName) {
		ConsumableItem generatedItem = null;
		
		
		
		return generatedItem;
	}	// End consumable generation.
	
	public static Equipment equipment (String equipName) {
		Equipment generatedEquipment = null;
		String generatedName = "No equipment";
		String generatedDescritption = "No equipment";
		int generatedAmount = 1;
		equipmentTypes equipType = null;
		Buff generatedBuff = new Buff("Equipment Buff", new BuffValues());
		
		
		if (equipName.contains(".") ) {
			generatedAmount = Integer.parseInt(equipName.split("/.")[1] ) ;
			equipName = equipName.split("/.")[0];
		}
		if (equipName.equalsIgnoreCase("noEquip-hat")) {
			generatedAmount = 0;
			equipType = equipmentTypes.hat;
		} else if (equipName.equalsIgnoreCase("noEquip-shirt")) {
			generatedAmount = 0;
			equipType = equipmentTypes.shirt;
		} else {
			// Throw an error.
		}
		
		if (generatedEquipment == null) {
			generatedEquipment = new Equipment(generatedAmount, generatedDescritption, generatedDescritption, generatedAmount, equipType, generatedBuff);
		}
		return generatedEquipment;
	}	// End equipment
	 
	public static Buff buff(String buffName) {
		BuffValues generatedBuff = new BuffValues();
		String generatedName = "Buff";
		// Basic buff set to do nothing and be named "Buff"
		
		if (buffName.equalsIgnoreCase("attack_small")) {
			generatedBuff.attackBuffNumber = 5;
		} else if (buffName.equalsIgnoreCase("megaBuff")) {
			
		}
		
		return new Buff(generatedName, generatedBuff);
	}	// End buff generation
	
}
