package items;

public class ManaPotion extends ConsumableItem {
	public ManaPotion(int itemAmount) {
		super(itemAmount);
	}
	
	public void use() {
		super.use();	// Is this correct? We want to override use().
		System.out.println("It has a delivious, fruity taste to it. Additionally, you die.");
		itemAmount--; 
		if (itemAmount <= 0) {
			//Add stuff for having none left
		}
	}

}
