#include <assert.h>
#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#include <direct.h>
#define make_dir(path) _mkdir(path)
#define remove_dir(path) _rmdir(path)
#else
#include <sys/stat.h>
#include <unistd.h>
#define make_dir(path) mkdir(path, 0700)
#define remove_dir(path) rmdir(path)
#endif

#include "../src/core/graph.h"
#include "../src/core/password.h"
#include "../src/manager/stats.h"
#include "../src/core/travels.h"
#include "../src/manager/userManager.h"
#include "../src/manager/backup.h"

extern double **parray;
extern char *guidePath[2 * MAXNUM];

static int nearly_equal(double left, double right)
{
        double diff = left - right;
        return diff > -0.001 && diff < 0.001;
}

static int file_exists(const char *path)
{
        FILE *fp = fopen(path, "r");
        if(fp == NULL)
        {
                return 0;
        }
        fclose(fp);
        return 1;
}

static int backup_file_if_exists(const char *path, const char *backupPath)
{
        remove(backupPath);
        if(!file_exists(path))
        {
                return 0;
        }
        assert(rename(path, backupPath) == 0);
        return 1;
}

static void restore_file_backup(const char *path, const char *backupPath, int hadBackup)
{
        remove(path);
        if(hadBackup)
        {
                assert(rename(backupPath, path) == 0);
        }
        else
        {
                remove(backupPath);
        }
}

static void set_vertex(ALGraph *graph, int index, const char *name)
{
        strncpy(graph->roadlist[index].data, name, sizeof(graph->roadlist[index].data) - 1);
        graph->roadlist[index].data[sizeof(graph->roadlist[index].data) - 1] = '\0';
        graph->roadlist[index].description[0] = '\0';
        graph->roadlist[index].ticketPrice = 0.0;
        graph->roadlist[index].openTime[0] = '\0';
        graph->roadlist[index].first = NULL;
}

static ALGraph create_sample_graph(void)
{
        ALGraph graph;
        initGraph(&graph);
        graph.nodenum = 3;
        set_vertex(&graph, 0, "A");
        set_vertex(&graph, 1, "B");
        set_vertex(&graph, 2, "C");
        assert(addEdge(&graph, 0, 1, 5));
        assert(addEdge(&graph, 1, 0, 5));
        assert(addEdge(&graph, 1, 2, 7));
        assert(addEdge(&graph, 2, 1, 7));
        assert(addEdge(&graph, 0, 2, 20));
        assert(addEdge(&graph, 2, 0, 20));
        return graph;
}

static void test_locate(void)
{
        ALGraph graph = create_sample_graph();
        assert(locate(graph, "A") == 0);
        assert(locate(graph, "C") == 2);
        assert(locate(graph, "X") == -1);
        assert(locate(graph, NULL) == -1);
        freeGraph(&graph);
}

static void test_add_edge_and_getlength(void)
{
        ALGraph graph = create_sample_graph();
        assert(graph.edgnum == 6);
        assert(getlength(graph, 0, 1) == 5);
        assert(getlength(graph, 1, 2) == 7);
        assert(getlength(graph, 0, 9) == 0);
        assert(!addEdge(NULL, 0, 1, 5));
        assert(!addEdge(&graph, 0, 2, 0));
        assert(!addEdge(&graph, -1, 2, 4));
        freeGraph(&graph);
}

static void test_matrix_and_floyd_shortest_path(void)
{
        int path[MAXNUM][MAXNUM];
        double shortpath[MAXNUM][MAXNUM];
        ALGraph graph = create_sample_graph();

        transToMatrix(&graph);
        assert(parray != NULL);
        assert(nearly_equal(parray[0][1], 5));
        assert(nearly_equal(parray[0][2], 20));

        shortPath(graph, path, shortpath);
        assert(nearly_equal(shortpath[0][2], 12));
        assert(path[0][2] == 1);

        freeMatrix(&graph);
        freeGraph(&graph);
}

static void test_dijkstra_shortest_path(void)
{
        int prev[MAXNUM];
        double distance = 0;
        ALGraph graph = create_sample_graph();

        assert(dijkstraShortestPath(graph, 0, 2, &distance, prev));
        assert(nearly_equal(distance, 12));
        assert(prev[2] == 1);
        assert(!dijkstraShortestPath(graph, -1, 2, &distance, prev));
        assert(!dijkstraShortestPath(graph, 0, 99, &distance, prev));

        assert(graphAddSpot(&graph, "D"));
        assert(!dijkstraShortestPath(graph, 0, 3, &distance, prev));

        freeGraph(&graph);
}

static void test_dijkstra_rejects_null_output_arguments(void)
{
        int prev[MAXNUM];
        double distance = 0;
        ALGraph graph = create_sample_graph();

        assert(!dijkstraShortestPath(graph, 0, 2, NULL, prev));
        assert(!dijkstraShortestPath(graph, 0, 2, &distance, NULL));
        freeGraph(&graph);
}

static void test_dijkstra_handles_same_start_and_end(void)
{
        int prev[MAXNUM];
        double distance = -1;
        ALGraph graph = create_sample_graph();

        assert(dijkstraShortestPath(graph, 1, 1, &distance, prev));
        assert(nearly_equal(distance, 0));
        assert(prev[1] == -1);
        freeGraph(&graph);
}

static void test_graph_crud(void)
{
        ALGraph graph;
        initGraph(&graph);

        assert(graphAddSpot(&graph, "A"));
        assert(graphAddSpot(&graph, "B"));
        assert(!graphAddSpot(&graph, "A"));
        assert(graphAddOrUpdateRoad(&graph, "A", "B", 9));
        assert(getlength(graph, locate(graph, "A"), locate(graph, "B")) == 9);
        assert(graphAddOrUpdateRoad(&graph, "A", "B", 4));
        assert(getlength(graph, locate(graph, "A"), locate(graph, "B")) == 4);
        assert(graphRenameSpot(&graph, "B", "C"));
        assert(locate(graph, "B") == -1);
        assert(locate(graph, "C") == 1);
        assert(graphDeleteRoad(&graph, "A", "C"));
        assert(getlength(graph, 0, 1) == 0);
        assert(graphDeleteSpot(&graph, "C"));
        assert(graph.nodenum == 1);

        freeGraph(&graph);
}

static void test_graph_add_spot_rejects_invalid_input(void)
{
        ALGraph graph;
        int i;

        initGraph(&graph);
        assert(!graphAddSpot(NULL, "A"));
        assert(!graphAddSpot(&graph, NULL));
        assert(!graphAddSpot(&graph, ""));
        for(i = 0; i < MAXNUM; i++)
        {
                char name[10];
                snprintf(name, sizeof(name), "N%d", i);
                assert(graphAddSpot(&graph, name));
        }
        assert(!graphAddSpot(&graph, "FULL"));
        freeGraph(&graph);
}

static void test_graph_rename_rejects_conflicts_and_missing_spots(void)
{
        ALGraph graph;

        initGraph(&graph);
        assert(graphAddSpot(&graph, "A"));
        assert(graphAddSpot(&graph, "B"));
        assert(!graphRenameSpot(NULL, "A", "C"));
        assert(!graphRenameSpot(&graph, NULL, "C"));
        assert(!graphRenameSpot(&graph, "A", NULL));
        assert(!graphRenameSpot(&graph, "X", "C"));
        assert(!graphRenameSpot(&graph, "A", "B"));
        assert(graphRenameSpot(&graph, "A", "C"));
        assert(locate(graph, "C") == 0);
        freeGraph(&graph);
}

static void test_graph_delete_spot_updates_edge_indices(void)
{
        ALGraph graph;

        initGraph(&graph);
        assert(graphAddSpot(&graph, "A"));
        assert(graphAddSpot(&graph, "B"));
        assert(graphAddSpot(&graph, "C"));
        assert(graphAddOrUpdateRoad(&graph, "A", "C", 8));
        assert(graphDeleteSpot(&graph, "B"));
        assert(graph.nodenum == 2);
        assert(locate(graph, "C") == 1);
        assert(getlength(graph, locate(graph, "A"), locate(graph, "C")) == 8);
        assert(!graphDeleteSpot(&graph, "X"));
        assert(!graphDeleteSpot(NULL, "A"));
        assert(!graphDeleteSpot(&graph, NULL));
        freeGraph(&graph);
}

static void test_graph_road_crud_rejects_invalid_input(void)
{
        ALGraph graph;

        initGraph(&graph);
        assert(graphAddSpot(&graph, "A"));
        assert(graphAddSpot(&graph, "B"));
        assert(!graphAddOrUpdateRoad(NULL, "A", "B", 1));
        assert(!graphAddOrUpdateRoad(&graph, NULL, "B", 1));
        assert(!graphAddOrUpdateRoad(&graph, "A", NULL, 1));
        assert(!graphAddOrUpdateRoad(&graph, "A", "X", 1));
        assert(!graphAddOrUpdateRoad(&graph, "A", "B", 0));
        assert(!graphDeleteRoad(NULL, "A", "B"));
        assert(!graphDeleteRoad(&graph, "A", "X"));
        assert(!graphDeleteRoad(&graph, NULL, "B"));
        freeGraph(&graph);
}

static void test_graph_delete_road_removes_middle_edge(void)
{
        ALGraph graph;

        initGraph(&graph);
        assert(graphAddSpot(&graph, "A"));
        assert(graphAddSpot(&graph, "B"));
        assert(graphAddSpot(&graph, "C"));
        assert(addEdge(&graph, 0, 1, 3));
        assert(addEdge(&graph, 0, 2, 4));
        assert(addEdge(&graph, 1, 0, 3));
        assert(addEdge(&graph, 2, 0, 4));
        assert(graphDeleteRoad(&graph, "A", "B"));
        assert(getlength(graph, 0, 1) == 0);
        assert(getlength(graph, 0, 2) == 4);
        freeGraph(&graph);
}

static void test_graph_matrix_handles_empty_and_null_graph(void)
{
        ALGraph graph;

        initGraph(&graph);
        transToMatrix(NULL);
        freeMatrix(NULL);
        transToMatrix(&graph);
        assert(parray == NULL);
        freeMatrix(&graph);
}

static void test_graph_get_edge_num_handles_invalid_input(void)
{
        ALGraph graph = create_sample_graph();

        assert(getEdgeNum(NULL, "A") == 0);
        assert(getEdgeNum(&graph, NULL) == 0);
        assert(getEdgeNum(&graph, "X") == 0);
        assert(getEdgeNum(&graph, "A") == 2);
        freeGraph(&graph);
}

static void test_guide_graph_ex_builds_expected_edges(void)
{
        ALGraph graph = create_sample_graph();
        ALGraph guide;

        initGraph(&guide);
        guidePath[0] = graph.roadlist[0].data;
        guidePath[1] = graph.roadlist[1].data;
        guidePath[2] = graph.roadlist[2].data;
        createGuideGraphEX(&graph, &guide, 3);
        assert(guide.nodenum == graph.nodenum);
        assert(getlength(guide, 0, 1) == 5);
        assert(getlength(guide, 1, 2) == 7);
        assert(getlength(guide, 0, 2) == 0);

        createGuideGraphEX(NULL, &guide, 3);
        createGuideGraphEX(&graph, NULL, 3);

        freeGraph(&guide);
        freeGraph(&graph);
}

static void test_password_hashing(void)
{
        char salt[PASSWORD_SALT_SIZE];
        char secondSalt[PASSWORD_SALT_SIZE];
        char hash[PASSWORD_HASH_SIZE];

        assert(generatePasswordSalt(salt, sizeof(salt)));
        assert(generatePasswordSalt(secondSalt, sizeof(secondSalt)));
        assert(strcmp(salt, secondSalt) != 0);
        assert(hashPassword("secret", salt, hash, sizeof(hash)));
        assert(strncmp(hash, "pbkdf2_sha256$", 14) == 0);
        assert(verifyPassword("secret", hash));
        assert(!verifyPassword("wrong", hash));
        assert(verifyPassword("legacy", "legacy"));
        assert(!verifyPassword("legacy", "other"));
        assert(!generatePasswordSalt(salt, 2));
        assert(!hashPassword(NULL, salt, hash, sizeof(hash)));
}

static void test_password_vectors_and_error_paths(void)
{
        char hash[PASSWORD_HASH_SIZE];
        char longHash[192];
        char smallOutput[8];
        const char *expected =
                "pbkdf2_sha256$10000$NaClNaClNaClNaCl$"
                "af966210366c8d767e53b9b07bbca665409a102dd70d9d6252d8c8a2cedcc79f";
        const char *longExpected =
                "pbkdf2_sha256$10000$ssssssssssssssssssssssssssssssssssssssssssssssssssss$"
                "3ecccde1b0f84e8ac62f4f42a0278d5a67a21955b856a7a7cfc54394affdd7d2";

        assert(hashPassword("password", "NaClNaClNaClNaCl", hash, sizeof(hash)));
        assert(strcmp(hash, expected) == 0);
        assert(verifyPassword("password", expected));

        assert(hashPassword("pppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppp",
                            "ssssssssssssssssssssssssssssssssssssssssssssssssssss", longHash, sizeof(longHash)));
        assert(strcmp(longHash, longExpected) == 0);

        assert(hashPassword("sp3c!@l chars", "SaltSaltSalt1234", hash, sizeof(hash)));
        assert(verifyPassword("sp3c!@l chars", hash));
        assert(!hashPassword("secret", NULL, hash, sizeof(hash)));
        assert(!hashPassword("secret", "SaltSaltSalt1234", NULL, sizeof(hash)));
        assert(!hashPassword("secret", "SaltSaltSalt1234", hash, 0));
        assert(!hashPassword("secret", "SaltSaltSalt1234", smallOutput, sizeof(smallOutput)));
        assert(!generatePasswordSalt(NULL, PASSWORD_SALT_SIZE));
        assert(!verifyPassword(NULL, expected));
        assert(!verifyPassword("password", NULL));
        assert(!verifyPassword("password", "pbkdf2_sha1$10000$NaClNaClNaClNaCl$digest"));
        assert(!verifyPassword("password", "pbkdf2_sha256$9999$NaClNaClNaClNaCl$digest"));
}

static void write_text_file(const char *path, const char *content)
{
        FILE *fp = fopen(path, "w");
        assert(fp != NULL);
        fputs(content, fp);
        fclose(fp);
}

static void test_graph_file_io_and_corruption(void)
{
        ALGraph graph = create_sample_graph();
        ALGraph loaded;
        const char *paramsPath = "test_graph_params.txt";
        const char *vertexPath = "test_graph_vertex.txt";
        const char *edgePath = "test_graph_edge.txt";

        initGraph(&loaded);
        assert(graphSaveToFiles(&graph, paramsPath, vertexPath, edgePath));
        assert(graphLoadFromFiles(&loaded, paramsPath, vertexPath, edgePath));
        assert(loaded.nodenum == 3);
        assert(getlength(loaded, locate(loaded, "A"), locate(loaded, "B")) == 5);
        freeGraph(&loaded);

        assert(!graphLoadFromFiles(&loaded, "missing_params.txt", vertexPath, edgePath));

        write_text_file(paramsPath, "200 1\n");
        assert(!graphLoadFromFiles(&loaded, paramsPath, vertexPath, edgePath));

        write_text_file(paramsPath, "3 2\n");
        write_text_file(vertexPath, "A\nB\nA\n");
        assert(!graphLoadFromFiles(&loaded, paramsPath, vertexPath, edgePath));

        write_text_file(vertexPath, "A\nB\nC\n");
        write_text_file(edgePath, "A B bad\n");
        assert(!graphLoadFromFiles(&loaded, paramsPath, vertexPath, edgePath));

        write_text_file(edgePath, "A X 5\n");
        assert(!graphLoadFromFiles(&loaded, paramsPath, vertexPath, edgePath));

        write_text_file(edgePath, "A B -1\n");
        assert(!graphLoadFromFiles(&loaded, paramsPath, vertexPath, edgePath));

        assert(!graphSaveToFiles(NULL, paramsPath, vertexPath, edgePath));
        assert(!graphSaveToFiles(&graph, NULL, vertexPath, edgePath));

        remove(paramsPath);
        remove(vertexPath);
        remove(edgePath);
        freeGraph(&loaded);
        freeGraph(&graph);
}

static void test_graph_default_path_wrappers_round_trip(void)
{
        ALGraph graph = create_sample_graph();
        ALGraph loaded;
        int hadParams = backup_file_if_exists("data/graphParams.txt", "data/graphParams.txt.testbak");
        int hadVertex = backup_file_if_exists("data/graphVertex.txt", "data/graphVertex.txt.testbak");
        int hadEdge = backup_file_if_exists("data/graphEdge.txt", "data/graphEdge.txt.testbak");

        initGraph(&loaded);
        saveGraph(&graph);
        assert(file_exists("data/graphParams.txt"));
        assert(file_exists("data/graphVertex.txt"));
        assert(file_exists("data/graphEdge.txt"));

        loadGraph(&loaded);
        assert(loaded.nodenum == graph.nodenum);
        assert(loaded.edgnum == graph.edgnum);
        assert(getlength(loaded, locate(loaded, "A"), locate(loaded, "C")) == 20);

        freeGraph(&loaded);
        restore_file_backup("data/graphParams.txt", "data/graphParams.txt.testbak", hadParams);
        restore_file_backup("data/graphVertex.txt", "data/graphVertex.txt.testbak", hadVertex);
        restore_file_backup("data/graphEdge.txt", "data/graphEdge.txt.testbak", hadEdge);
        freeGraph(&graph);
}

static void test_graph_file_loader_rejects_extra_edge_fields(void)
{
        ALGraph loaded;
        const char *paramsPath = "test_graph_params_extra.txt";
        const char *vertexPath = "test_graph_vertex_extra.txt";
        const char *edgePath = "test_graph_edge_extra.txt";

        initGraph(&loaded);
        write_text_file(paramsPath, "2 1\n");
        write_text_file(vertexPath, "A\nB\n");
        write_text_file(edgePath, "A B 5 extra\n");
        assert(!graphLoadFromFiles(&loaded, paramsPath, vertexPath, edgePath));

        remove(paramsPath);
        remove(vertexPath);
        remove(edgePath);
        freeGraph(&loaded);
}

static void test_graph_file_saver_rejects_unwritable_paths(void)
{
        ALGraph graph = create_sample_graph();

        assert(!graphSaveToFiles(&graph, ".", "test_vertex_unwritten.txt", "test_edge_unwritten.txt"));
        assert(!graphSaveToFiles(&graph, "test_params_partial.txt", ".", "test_edge_unwritten.txt"));
        assert(!graphSaveToFiles(&graph, "test_params_partial.txt", "test_vertex_partial.txt", "."));
        remove("test_params_partial.txt");
        remove("test_vertex_partial.txt");
        remove("test_vertex_unwritten.txt");
        remove("test_edge_unwritten.txt");
        freeGraph(&graph);
}

static void test_user_management_failure_and_migration(void)
{
        const char *userPath = "test_user.txt";
        char username[COLUMNLENGTH] = "alice";
        char password[COLUMNLENGTH] = "secret";
        char wrongPassword[COLUMNLENGTH] = "wrong";
        char missingUser[COLUMNLENGTH] = "nobody";
        char fileUser[COLUMNLENGTH];
        char filePassword[PASSWORD_FIELD_LENGTH];
        FILE *fp;

        remove(userPath);
        assert(validateUserInFile(userPath, username, password, 0) == 2);
        assert(!saveUserToFile(userPath, "", password));
        assert(saveUserToFile(userPath, username, password));
        assert(!saveUserToFile(userPath, username, password));
        assert(validateUserInFile(userPath, username, password, 0) == 1);
        assert(validateUserInFile(userPath, username, wrongPassword, 0) == 3);
        assert(validateUserInFile(userPath, missingUser, password, 0) == 2);

        write_text_file(userPath, "legacy oldpass\n");
        assert(validateUserInFile(userPath, "legacy", "oldpass", 1) == 1);
        fp = fopen(userPath, "r");
        assert(fp != NULL);
        assert(fscanf(fp, "%9s %127s", fileUser, filePassword) == 2);
        fclose(fp);
        assert(strcmp(fileUser, "legacy") == 0);
        assert(strncmp(filePassword, "pbkdf2_sha256$", 14) == 0);
        assert(validateUserInFile(userPath, "legacy", "bad", 1) == 3);

        remove(userPath);
}

static void test_user_validation_skips_malformed_records(void)
{
        const char *userPath = "test_user_malformed.txt";

        write_text_file(userPath, "x\nbob legacyPass\n");
        assert(validateUserInFile(userPath, "bob", "legacyPass", 0) == 1);
        assert(validateUserInFile(userPath, "bob", "bad", 0) == 3);
        assert(validateUserInFile(userPath, "missing", "legacyPass", 0) == 2);
        remove(userPath);
}

static void test_user_save_rejects_null_and_empty_fields(void)
{
        const char *userPath = "test_user_invalid.txt";

        remove(userPath);
        assert(!saveUserToFile(NULL, "alice", "secret"));
        assert(!saveUserToFile(userPath, NULL, "secret"));
        assert(!saveUserToFile(userPath, "alice", NULL));
        assert(!saveUserToFile(userPath, "", "secret"));
        assert(!saveUserToFile(userPath, "alice", ""));
        remove(userPath);
}

static void test_user_validation_rejects_null_arguments(void)
{
        assert(validateUserInFile(NULL, "alice", "secret", 0) == 2);
        assert(validateUserInFile("missing_user_file.txt", NULL, "secret", 0) == 2);
        assert(validateUserInFile("missing_user_file.txt", "alice", NULL, 0) == 2);
}

static void test_user_default_wrappers_reject_empty_credentials(void)
{
        char empty[COLUMNLENGTH] = "";
        char username[COLUMNLENGTH] = "missing";
        char password[COLUMNLENGTH] = "secret";

        assert(!saveUser(empty, empty));
        assert(validateUser(username, password) != 1);
}

static void test_user_save_reports_unwritable_user_path(void)
{
        assert(!saveUserToFile(".", "blocked", "secret"));
}

static void test_user_role_permissions(void)
{
        const char *rolePath = "test_user_roles.txt";

        write_text_file(rolePath, "alice hash user\nroot hash admin\nlegacy hash\nunknown hash manager\n");
        userManagerSetCurrentUser(NULL);
        assert(userManagerGetCurrentRole() == USER_ROLE_GUEST);
        assert(!userCanManageGraph());
        assert(strcmp(userManagerTestRoleToString(USER_ROLE_GUEST), "guest") == 0);
        assert(strcmp(userManagerTestRoleToString(USER_ROLE_ADMIN), "admin") == 0);

        assert(userManagerRoleForUsername("admin") == USER_ROLE_USER);
        assert(userManagerRoleForUserInFile(NULL, "root") == USER_ROLE_GUEST);
        assert(userManagerRoleForUserInFile(rolePath, NULL) == USER_ROLE_GUEST);
        assert(userManagerRoleForUserInFile("missing_role_file.txt", "root") == USER_ROLE_GUEST);
        assert(userManagerRoleForUserInFile(rolePath, "missing") == USER_ROLE_GUEST);
        assert(userManagerRoleForUserInFile(rolePath, "legacy") == USER_ROLE_USER);
        assert(userManagerRoleForUserInFile(rolePath, "root") == USER_ROLE_ADMIN);
        assert(userManagerRoleForUserInFile(rolePath, "unknown") == USER_ROLE_USER);

        userManagerSetCurrentUserFromFile(rolePath, "alice");
        assert(strcmp(userManagerGetCurrentUser(), "alice") == 0);
        assert(userManagerGetCurrentRole() == USER_ROLE_USER);
        assert(!userCanManageGraph());

        userManagerSetCurrentUserFromFile(rolePath, "root");
        assert(userManagerGetCurrentRole() == USER_ROLE_ADMIN);
        assert(userCanManageGraph());

        userManagerSetCurrentUserFromFile("missing_role_file.txt", "fallback");
        assert(userManagerGetCurrentRole() == USER_ROLE_USER);
        remove(rolePath);
}

static void test_user_save_with_role_and_admin_audit(void)
{
        const char *userPath = "test_user_save_role.txt";

        remove(userPath);
        assert(saveUserToFileWithRole(userPath, "root", "secret", USER_ROLE_ADMIN));
        assert(validateUserInFile(userPath, "root", "secret", 0) == 1);
        assert(userManagerRoleForUserInFile(userPath, "root") == USER_ROLE_ADMIN);

        make_dir("audit");
        userManagerSetCurrentUserFromFile(userPath, "root");
        userManagerLogAdminAction("test.action", "target", "success");
        userManagerLogAdminAction(NULL, NULL, NULL);
        assert(userCanManageGraph());
        remove(userPath);
}

static void test_user_audit_query_outputs_known_paths(void)
{
        const char *nonEmptyPath = "test_audit_nonempty.log";
        const char *emptyPath = "test_audit_empty.log";

        write_text_file(nonEmptyPath, "line1\nline2\n");
        write_text_file(emptyPath, "");
        userManagerPrintMigrationAudit(NULL, 0);
        userManagerPrintMigrationAudit("missing_audit.log", 3);
        userManagerPrintMigrationAudit(emptyPath, 3);
        userManagerPrintAdminAudit(nonEmptyPath, 1);
        userManagerPrintAdminAudit(NULL, 1);
        remove(nonEmptyPath);
        remove(emptyPath);
}

static void test_user_migration_helper_file_utilities(void)
{
        char sanitized[32];
        char backupPath[160];
        const char *sourcePath = "test_user_copy_source.txt";
        const char *targetPath = "test_user_copy_target.txt";

        write_text_file(sourcePath, "copy me\n");
        assert(userManagerTestCopyFile(sourcePath, targetPath));
        assert(validateUserInFile(targetPath, "copy", "me", 0) == 1);
        assert(!userManagerTestCopyFile(NULL, targetPath));
        assert(!userManagerTestCopyFile("missing_copy_source.txt", targetPath));
        assert(!userManagerTestCopyFile(sourcePath, "."));

        userManagerTestSanitizePathName("dir/user:file.txt", sanitized, sizeof(sanitized));
        assert(strcmp(sanitized, "dir_user_file.txt") == 0);
        userManagerTestSanitizePathName(NULL, sanitized, sizeof(sanitized));
        assert(strcmp(sanitized, "") == 0);
        userManagerTestSanitizePathName("ignored", sanitized, 0);

        assert(!userManagerTestBuildMigrationBackupPath(NULL, backupPath, sizeof(backupPath)));
        assert(!userManagerTestBuildMigrationBackupPath(sourcePath, NULL, sizeof(backupPath)));
        assert(!userManagerTestBuildMigrationBackupPath(sourcePath, backupPath, 4));
        assert(userManagerTestBuildMigrationBackupPath(sourcePath, backupPath, sizeof(backupPath)));
        assert(strncmp(backupPath, "backup/user/", 12) == 0);

        remove(sourcePath);
        remove(targetPath);
}

static void test_user_migration_helper_rejects_invalid_inputs(void)
{
        char longPath[130];

        memset(longPath, 'a', 124);
        longPath[124] = '\0';
        assert(!userManagerTestMigrateLegacyUserPassword(NULL, "legacy", "newhash"));
        assert(!userManagerTestMigrateLegacyUserPassword("test_user_migrate.txt", NULL, "newhash"));
        assert(!userManagerTestMigrateLegacyUserPassword("test_user_migrate.txt", "legacy", NULL));
        assert(!userManagerTestMigrateLegacyUserPassword(longPath, "legacy", "newhash"));
        assert(!userManagerTestMigrateLegacyUserPassword("missing_migrate_file.txt", "legacy", "newhash"));
}

static void test_user_migration_helper_preserves_other_users(void)
{
        const char *userPath = "test_user_migrate_preserve.txt";

        write_text_file(userPath, "broken-line\nother keep admin\nlegacy oldpass user\n");
        assert(userManagerTestMigrateLegacyUserPassword(userPath, "legacy", "newhash"));
        assert(validateUserInFile(userPath, "other", "keep", 0) == 1);
        assert(validateUserInFile(userPath, "legacy", "newhash", 0) == 1);
        assert(validateUserInFile("backup/user/migration.log", "missing", "missing", 0) == 2);
        remove(userPath);
}

static void test_user_migration_helper_reports_missing_target(void)
{
        const char *userPath = "test_user_migrate_missing_target.txt";

        write_text_file(userPath, "other keep\n");
        assert(!userManagerTestMigrateLegacyUserPassword(userPath, "legacy", "newhash"));
        remove(userPath);
        remove("test_user_migrate_missing_target.txt.tmp");
}

static void test_user_migration_helper_reports_temp_open_failure(void)
{
        const char *userPath = "test_user_migrate_temp_fail.txt";
        const char *tempPath = "test_user_migrate_temp_fail.txt.tmp";

        remove_dir(tempPath);
        write_text_file(userPath, "legacy oldpass\n");
        assert(make_dir(tempPath) == 0);
        assert(!userManagerTestMigrateLegacyUserPassword(userPath, "legacy", "newhash"));
        remove(userPath);
        remove_dir(tempPath);
}

static void test_stats_module(void)
{
        ALGraph graph = create_sample_graph();
        SystemStats stats;

        statsInit();
        statsRecordStart();
        statsRecordLogin();
        statsRecordRegistration();
        statsRecordPathQuery();
        statsRecordSpotAccess(&graph, "A");
        statsRecordSpotAccess(&graph, "C");
        stats = statsGetSnapshot();

        assert(stats.totalStarts == 1);
        assert(stats.totalLogins == 1);
        assert(stats.totalRegistrations == 1);
        assert(stats.totalPathQueries == 1);
        assert(stats.spotAccess[0] == 1);
        assert(stats.spotAccess[2] == 1);

        freeGraph(&graph);
}

static void test_traversal_loop_and_reporting(void)
{
        int pathMatrix[MAXNUM][MAXNUM];
        double shortpath[MAXNUM][MAXNUM];
        ALGraph graph = create_sample_graph();
        ALGraph dag;

        assert(isedg(graph, "A", "B"));
        assert(!isedg(graph, "A", "X"));
        assert(getEdgeNum(&graph, "A") == 2);
        DFSTraverse(graph);
        transToMatrix(&graph);
        shortPath(graph, pathMatrix, shortpath);
        printPath(graph, pathMatrix, shortpath, 0, 2);
        printf("\n");

        initGraph(&dag);
        assert(graphAddSpot(&dag, "A"));
        assert(graphAddSpot(&dag, "B"));
        assert(graphAddSpot(&dag, "C"));
        assert(addEdge(&dag, 0, 1, 1));
        assert(addEdge(&dag, 1, 2, 1));
        assert(islooptest(dag) == 1);
        assert(addEdge(&dag, 2, 0, 1));
        assert(islooptest(dag) == 0);

        freeMatrix(&graph);
        freeGraph(&dag);
        freeGraph(&graph);
}

static void test_travels_edge_queries_reject_invalid_names(void)
{
        ALGraph graph = create_sample_graph();

        assert(!isedg(graph, NULL, "A"));
        assert(!isedg(graph, "A", NULL));
        assert(!isedg(graph, "X", "A"));
        assert(getlength(graph, -1, 0) == 0);
        assert(getlength(graph, 0, -1) == 0);
        assert(getlength(graph, 0, graph.nodenum) == 0);
        freeGraph(&graph);
}

static void test_print_path(void)
{
        int pathMatrix[MAXNUM][MAXNUM];
        double shortpath[MAXNUM][MAXNUM];
        ALGraph graph = create_sample_graph();

        transToMatrix(&graph);
        shortPath(graph, pathMatrix, shortpath);
        printPath(graph, pathMatrix, shortpath, 0, 2);

        pathMatrix[0][2] = -1;
        printPath(graph, pathMatrix, shortpath, 0, 2);

        freeMatrix(&graph);
        freeGraph(&graph);
}

static void test_stats_persistence_and_report(void)
{
        ALGraph graph = create_sample_graph();
        SystemStats stats;

        statsInit();
        statsRecordStart();
        statsRecordPathQuery();
        statsRecordSpotAccess(&graph, "B");
        statsSave(&graph);
        statsInit();
        statsLoad(&graph);
        stats = statsGetSnapshot();
        assert(stats.totalStarts == 1);
        assert(stats.totalPathQueries == 1);
        assert(stats.spotAccess[1] == 1);
        statsPrintReport(&graph);
        remove("stats.txt");

        freeGraph(&graph);
}

static void test_stats_error_and_empty_graph_branches(void)
{
        ALGraph graph;
        SystemStats stats;
        int hadStats = backup_file_if_exists("stats.txt", "stats.txt.testbak");

        initGraph(&graph);
        statsInit();
        statsRecordSpotAccess(NULL, "A");
        statsRecordSpotAccess(&graph, NULL);
        statsRecordSpotAccess(&graph, "missing");
        stats = statsGetSnapshot();
        assert(stats.spotAccess[0] == 0);

        statsPrintReport(NULL);
        statsPrintReport(&graph);
        statsSave(NULL);
        assert(file_exists("stats.txt"));
        remove("stats.txt");

        assert(make_dir("stats.txt") == 0);
        statsSave(&graph);
        assert(remove_dir("stats.txt") == 0);

        restore_file_backup("stats.txt", "stats.txt.testbak", hadStats);
        freeGraph(&graph);
}

static void test_input_helpers_validate_tokens(void)
{
        const char *inputPath = "test_input_tokens.txt";
        char text[4];
        int value = 0;

        assert(!readString(NULL, sizeof(text)));
        assert(!readString(text, 0));

        write_text_file(inputPath, "abcdef 123 invalid");
        assert(freopen(inputPath, "r", stdin) != NULL);

        assert(readString(text, sizeof(text)));
        assert(strcmp(text, "abc") == 0);
        assert(readInt(&value));
        assert(value == 123);
        assert(!readInt(&value));
        assert(!readString(text, sizeof(text)));

        assert(fclose(stdin) == 0);
        remove(inputPath);
}

static void test_graph_spot_info(void)
{
        ALGraph graph;
        initGraph(&graph);

        assert(graphAddSpot(&graph, "SpotA"));
        assert(graphUpdateSpotInfo(&graph, "SpotA", "A beautiful scenic spot", 50.0, "9:00-17:00"));
        assert(!graphUpdateSpotInfo(&graph, "Missing", "Desc", 10.0, "Time"));
        assert(!graphUpdateSpotInfo(NULL, "SpotA", "Desc", 10.0, "Time"));
        assert(!graphUpdateSpotInfo(&graph, NULL, "Desc", 10.0, "Time"));

        assert(graphAddSpot(&graph, "SpotB"));
        assert(graphUpdateSpotInfo(&graph, "SpotB", "", 0.0, ""));

        graphPrintSpotInfo(&graph, "SpotA");
        graphPrintSpotInfo(&graph, "Missing");
        graphPrintSpotInfo(NULL, "SpotA");

        freeGraph(&graph);
}

static void test_backup_restore(void)
{
        ALGraph graph = create_sample_graph();
        ALGraph restored;
        initGraph(&restored);

        assert(backupCreate(&graph, NULL));
        backupList();
        assert(backupDelete("nonexistent_backup"));

        assert(backupRestore("nonexistent_backup", &restored, NULL) == 0);

        freeGraph(&restored);
        freeGraph(&graph);
}

static void test_user_change_password(void)
{
        const char *userPath = "test_user_change_pwd.txt";

        remove(userPath);
        assert(saveUserToFile(userPath, "testuser", "oldpass"));
        assert(validateUserInFile(userPath, "testuser", "oldpass", 0) == 1);

        assert(!userChangePassword(NULL, "oldpass", "newpass"));
        assert(!userChangePassword("testuser", NULL, "newpass"));
        assert(!userChangePassword("testuser", "oldpass", NULL));
        assert(!userChangePassword("testuser", "", "newpass"));
        assert(!userChangePasswordInFile(userPath, "testuser", "wrongpass", "newpass"));
        assert(!userChangePasswordInFile(userPath, "testuser", "oldpass", "oldpass"));

        assert(userChangePasswordInFile(userPath, "testuser", "oldpass", "newpass"));
        assert(validateUserInFile(userPath, "testuser", "newpass", 0) == 1);
        assert(validateUserInFile(userPath, "testuser", "oldpass", 0) == 3);

        assert(!userChangePasswordInFile(userPath, "missing", "oldpass", "newpass"));

        remove(userPath);
}

static void test_export_guide_route(void)
{
        ALGraph graph = create_sample_graph();
        ALGraph guide;
        initGraph(&guide);

        guidePath[0] = graph.roadlist[0].data;
        guidePath[1] = graph.roadlist[1].data;
        guidePath[2] = graph.roadlist[2].data;
        createGuideGraphEX(&graph, &guide, 3);

        assert(exportGuideRoute(guide, "test_guide_export.txt"));
        assert(file_exists("test_guide_export.txt"));
        assert(!exportGuideRoute(guide, NULL));

        ALGraph empty;
        initGraph(&empty);
        assert(!exportGuideRoute(empty, "test_empty_export.txt"));

        remove("test_guide_export.txt");

        freeGraph(&guide);
        freeGraph(&graph);
        freeGraph(&empty);
}

int main(void)
{
        test_locate();
        test_add_edge_and_getlength();
        test_matrix_and_floyd_shortest_path();
        test_dijkstra_shortest_path();
        test_dijkstra_rejects_null_output_arguments();
        test_dijkstra_handles_same_start_and_end();
        test_graph_crud();
        test_graph_add_spot_rejects_invalid_input();
        test_graph_rename_rejects_conflicts_and_missing_spots();
        test_graph_delete_spot_updates_edge_indices();
        test_graph_road_crud_rejects_invalid_input();
        test_graph_delete_road_removes_middle_edge();
        test_graph_matrix_handles_empty_and_null_graph();
        test_graph_get_edge_num_handles_invalid_input();
        test_guide_graph_ex_builds_expected_edges();
        test_graph_file_io_and_corruption();
        test_graph_default_path_wrappers_round_trip();
        test_graph_file_loader_rejects_extra_edge_fields();
        test_graph_file_saver_rejects_unwritable_paths();
        test_password_hashing();
        test_password_vectors_and_error_paths();
        test_user_management_failure_and_migration();
        test_user_validation_skips_malformed_records();
        test_user_save_rejects_null_and_empty_fields();
        test_user_validation_rejects_null_arguments();
        test_user_default_wrappers_reject_empty_credentials();
        test_user_save_reports_unwritable_user_path();
        test_user_role_permissions();
        test_user_save_with_role_and_admin_audit();
        test_user_audit_query_outputs_known_paths();
        test_user_migration_helper_file_utilities();
        test_user_migration_helper_rejects_invalid_inputs();
        test_user_migration_helper_preserves_other_users();
        test_user_migration_helper_reports_missing_target();
        test_user_migration_helper_reports_temp_open_failure();
        test_stats_module();
        test_traversal_loop_and_reporting();
        test_travels_edge_queries_reject_invalid_names();
        test_print_path();
        test_stats_persistence_and_report();
        test_stats_error_and_empty_graph_branches();
        test_input_helpers_validate_tokens();
        test_graph_spot_info();
        test_backup_restore();
        test_user_change_password();
        test_export_guide_route();
        printf("All tests passed.\n");
        return 0;
}
