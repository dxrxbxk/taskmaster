#include "taskmaster/other/daemon.hpp"
#include "taskmaster/log/logger.hpp"
#include "taskmaster/taskmaster.hpp"


// ╭───────────────────────────────────────╮
// │ le cube de johnny le taskmaster !     │
// ╰───────────────────────────────────────╯

/* entry point */
auto main(void) -> int {

	try {
		sm::daemonize(sm::taskmaster::run);
		//sm::taskmaster::run();
	}

	catch (const sm::exception& e) {
		sm::logger::error(e.what());
		return EXIT_FAILURE;
	}

	catch (const std::exception& e) {
		sm::logger::error(e.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}



/*
auto list_processes(void) -> void {

	struct ::dirent* entry;

	sm::unique_dir proc{"/proc"};

	while ((entry = proc.read()) != nullptr) {

		// Vérifier si le nom du répertoire est un nombre (PID)
		if (not isdigit(entry->d_name[0])) {
			continue;
		}

		char path[256];
		char name[256];
		snprintf(path, sizeof(path), "/proc/%s/status", entry->d_name);

		FILE *status_file = fopen(path, "r");
		if (status_file == NULL) {
			continue;
		}

		while (fgets(name, sizeof(name), status_file)) {
			if (strncmp(name, "Name:", 5) == 0) {
				// Extraire le nom du processus
				char process_name[256];
				sscanf(name, "Name:\t%255s", process_name);
				printf("%-10s %-30s\n", entry->d_name, process_name);
				break;
			}
		}
		fclose(status_file);
	}

}
*/

