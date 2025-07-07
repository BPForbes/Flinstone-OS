#include <CUnit/Basic.h>
#include "fs.h"
#include "interpreter.h"

/* Helper to run and assert */
static int run(const char *cmd) {
    return execute_command_str(cmd);
}

void test_mkdisk_batch(void) {
    CU_ASSERT_EQUAL(run("-mkdisk diskA 8 16"), 0);
    remove("diskA_disk.txt");
}

void test_mkdisk_interactive(void) {
    CU_ASSERT_EQUAL(run("-mkdisk diskI 8 16 -y"), 0);
    remove("diskI_disk.txt");
}

void test_D_and_R_commands(void) {
    CU_ASSERT_EQUAL(run("-D test_dir"), 0);
    CU_ASSERT_EQUAL(run("cd test_dir"), 0);
    CU_ASSERT_EQUAL(run("cd .."), 0);
    CU_ASSERT_EQUAL(run("-R test_dir"), 0);
    CU_ASSERT_NOT_EQUAL(run("cd test_dir"), 0);
}

void test_dir_command(void) {
    CU_ASSERT_EQUAL(run("dir"), 0);
}

void test_cd_and_pwd_command(void) {
    CU_ASSERT_EQUAL(run("-D a"), 0);
    CU_ASSERT_EQUAL(run("cd a"), 0);
    CU_ASSERT_EQUAL(run("pwd"), 0);
    CU_ASSERT_EQUAL(run("cd .."), 0);
    CU_ASSERT_EQUAL(run("-R a"), 0);
}

void test_init_command(void) {
    CU_ASSERT_EQUAL(run("-init 100 64"), 0);
    CU_ASSERT_EQUAL(g_total_clusters, 100);
    CU_ASSERT_EQUAL(g_cluster_size, 64);
}

int main() {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Flintstone Suite", NULL, NULL);

    CU_ADD_TEST(suite, test_mkdisk_batch);
    CU_ADD_TEST(suite, test_mkdisk_interactive);
    CU_ADD_TEST(suite, test_D_and_R_commands);
    CU_ADD_TEST(suite, test_dir_command);
    CU_ADD_TEST(suite, test_cd_and_pwd_command);
    CU_ADD_TEST(suite, test_init_command);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0;
}
