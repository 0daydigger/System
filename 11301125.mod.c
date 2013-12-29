#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x84b3ef83, "module_layout" },
	{ 0x15692c87, "param_ops_int" },
	{ 0x39d636e4, "cdev_del" },
	{ 0x2ad04077, "remove_proc_entry" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0x29537c9e, "alloc_chrdev_region" },
	{ 0xd78b6795, "create_proc_entry" },
	{ 0x43ff78f1, "kmem_cache_alloc_trace" },
	{ 0x1cb9af37, "kmalloc_caches" },
	{ 0xd1515064, "cdev_add" },
	{ 0x685acc2b, "cdev_init" },
	{ 0xd8e484f0, "register_chrdev_region" },
	{ 0xdd0a2ba2, "strlcat" },
	{ 0x37a0cba, "kfree" },
	{ 0x2f287f0d, "copy_to_user" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0x79aa04a2, "get_random_bytes" },
	{ 0x91715312, "sprintf" },
	{ 0x636c6e93, "force_sig" },
	{ 0xd1446a71, "current_task" },
	{ 0x50eedeb8, "printk" },
	{ 0xb4390f9a, "mcount" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "C65B428FC94D28E33F23093");
