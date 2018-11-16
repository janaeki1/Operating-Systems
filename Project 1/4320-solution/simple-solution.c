#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/types.h>

struct birthday 
{	
	char *name;
	int month;
	int day;
	int year;	
	struct list_head list;
};

/**
 * The following defines and initializes a list_head object named birthday_list
 */
static LIST_HEAD(birthday_list);

int simple_init(void)
{

       printk(KERN_INFO "Loading Module\n");

       //Pointers for memory allocation
	struct birthday *john;
	john = kmalloc(sizeof(*john),GFP_KERNEL);
	john->name = "John";
	john->month = 1;
	john->day = 2;
	john->year = 1993;
	INIT_LIST_HEAD(&john->list);
	
	struct birthday *lucas;
	lucas = kmalloc(sizeof(*lucas),GFP_KERNEL);
	lucas->name = "Lucas";
	lucas->month = 3;
	lucas->day = 4;
	lucas->year = 1995;
	INIT_LIST_HEAD(&lucas->list);
	
	struct birthday *sarah;
	sarah = kmalloc(sizeof(*sarah),GFP_KERNEL);
	sarah->name = "Sarah";
	sarah->month = 4;
	sarah->day = 5;
	sarah->year = 1996;
	INIT_LIST_HEAD(&sarah->list);
	
	struct birthday *matt;
	matt = kmalloc(sizeof(*matt),GFP_KERNEL);
	matt->name = "Matt";
	matt->month = 5;
	matt->day = 6;
	matt->year = 1997;
	INIT_LIST_HEAD(&matt->list);
	
	struct birthday *kelly;
	kelly = kmalloc(sizeof(*kelly),GFP_KERNEL);
	kelly->name = "Kelly";
	kelly->month = 6;
	kelly->day = 7;
	kelly->year = 1998;
	INIT_LIST_HEAD(&kelly->list);
	
	struct birthday *marcus;
	marcus = kmalloc(sizeof(*marcus),GFP_KERNEL);
	marcus->name = "Marcus";
	marcus->month = 7;
	marcus->day = 8;
	marcus->year = 1999;
	INIT_LIST_HEAD(&marcus->list);
		

	//Adds new nodes to linked list
	list_add_tail(&john->list, &birthday_list);
	list_add_tail(&lucas->list, &birthday_list);
	list_add_tail(&sarah->list, &birthday_list);
	list_add_tail(&matt->list, &birthday_list);
	list_add_tail(&kelly->list, &birthday_list);
	list_add_tail(&marcus->list, &birthday_list);

	//Traverses the linked list
	struct birthday *ptr, *next, *max;

	list_for_each_entry(ptr, &birthday_list, list){
		/*On each iteration, ptr points 
		to the next birthday struct*/
		printk(KERN_INFO "%s's Birthday: Month-%d Day-%d Year-%d\n", ptr->name, ptr->month, ptr->day, ptr->year);
	}
	
	//Deletes the node of the youngest student
	list_for_each_entry(ptr, &birthday_list, list){
		max = &birthday_list;
		if(ptr->year > max->year){
			max = ptr;
		}
   }
   list_del(&max->list);
   
   //Traverses the new linked list
   list_for_each_entry(ptr, &birthday_list, list){
		/*On each iteration, ptr points 
		to the next birthday struct*/
		printk(KERN_INFO "%s's Birthday: Month-%d Day-%d Year-%d\n", ptr->name, ptr->month, ptr->day, ptr->year);
	}
       
       return 0;
}

void simple_exit(void) {
	
	
	printk(KERN_INFO "Removing Module\n");

	
	//Removes each element while traversing the linked list
	struct birthday *ptr, *next;

	list_for_each_entry_safe(ptr, next, &birthday_list, list){
		/*On each iteration, ptr points 
		to the next birthday struct*/
		printk(KERN_INFO "Removing %s's Birthday\n", ptr->name);
		list_del(&ptr->list);
		kfree(ptr);
	}
	

}

module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Kernel Data Structures");
MODULE_AUTHOR("SGG");
