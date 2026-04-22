#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>
#include <sstream>
#include <iomanip>

struct PatchInfo {
    size_t offset;
    std::string patch_file;
};

std::vector<PatchInfo> parse_arguments(int argc, char* argv[]) {
    std::vector<PatchInfo> patches;

    if (argc < 4 || ((argc - 3) % 2 != 0)) {
        std::cout << "Usage: apply_patch <input.bin> <output.bin> <offset1> <patch1.bin> [<offset2> <patch2.bin> ...]\n";
        std::cout << "Example: apply_patch input.bin output.bin 0x1000 patch1.bin 0x2000 patch2.bin 0x3000 patch3.bin\n";
        std::cout << "Multiple patches will be applied to the same file.\n\n";

        std::cout << "Arguments provided (" << argc << "):\n";
        for (int i = 0; i < argc; i++) {
            std::cout << "  argv[" << i << "] = \"" << (argv[i] ? argv[i] : "(null)") << "\"\n";
        }

        return patches;
    }

    for (int i = 3; i < argc; i += 2) {
        PatchInfo patch;

        // Parse offset (support hex with 0x prefix)
        std::string offset_str = argv[i];
        size_t offset;
        if (offset_str.length() > 2 && offset_str[0] == '0' && (offset_str[1] == 'x' || offset_str[1] == 'X')) {
            offset = std::stoul(offset_str.substr(2), nullptr, 16);
        }
        else {
            offset = std::stoul(offset_str, nullptr, 0); // auto-detect base
        }

        patch.offset = offset;
        patch.patch_file = argv[i + 1];
        patches.push_back(patch);
    }

    return patches;
}

std::vector<uint8_t> read_binary_file(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        throw std::runtime_error("Failed to read file: " + filename);
    }

    return buffer;
}

void write_binary_file(const std::string& filename, const std::vector<uint8_t>& data) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot create file: " + filename);
    }

    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

void apply_patches(std::vector<uint8_t>& data, const std::vector<PatchInfo>& patches) {
    for (const auto& patch : patches) {
        std::cout << "\nApplying patch: " << patch.patch_file << "\n";
        std::cout << "  Offset: 0x" << std::hex << patch.offset << std::dec << "\n";

        // Read patch bytes
        std::vector<uint8_t> patch_bytes = read_binary_file(patch.patch_file);
        std::cout << "  Patch size: " << patch_bytes.size() << " bytes\n";

        // Check if patch fits
        if (patch.offset + patch_bytes.size() > data.size()) {
            std::stringstream error_msg;
            error_msg << "Patch doesn't fit in file!\n"
                << "  File size: " << data.size() << " bytes\n"
                << "  Patch offset: 0x" << std::hex << patch.offset << std::dec << "\n"
                << "  Patch size: " << patch_bytes.size() << " bytes\n"
                << "  Required space: " << (patch.offset + patch_bytes.size()) << " bytes";
            throw std::runtime_error(error_msg.str());
        }

        // Apply patch
        std::copy(patch_bytes.begin(), patch_bytes.end(), data.begin() + patch.offset);

        std::cout << "  ✓ Applied successfully\n";

        // Show first few bytes for verification
        if (!patch_bytes.empty()) {
            std::cout << "  First 16 bytes of patch: ";
            size_t show_bytes = std::min<size_t>(patch_bytes.size(), 16);
            for (size_t i = 0; i < show_bytes; i++) {
                std::cout << std::hex << std::setw(2) << std::setfill('0')
                    << static_cast<int>(patch_bytes[i]) << " ";
            }
            std::cout << std::dec << "\n";
        }
    }
}

int main(int argc, char* argv[]) {
    try {
        // Parse command line arguments
        std::vector<PatchInfo> patches = parse_arguments(argc, argv);
        if (patches.empty()) {
            return 1; // Usage was shown
        }

        std::string input_file = argv[1];
        std::string output_file = argv[2];

        std::cout << "Input file: " << input_file << "\n";
        std::cout << "Output file: " << output_file << "\n";
        std::cout << "Number of patches to apply: " << patches.size() << "\n";

        // Read input file
        std::cout << "\nReading input file...\n";
        std::vector<uint8_t> data = read_binary_file(input_file);
        std::cout << "  File size: " << data.size() << " bytes ("
            << (data.size() / 1024) << " KB)\n";

        // Apply all patches
        apply_patches(data, patches);

        // Write output file
        std::cout << "\nWriting output file...\n";
        write_binary_file(output_file, data);
        std::cout << "  ✓ Output written: " << output_file << "\n";

        std::cout << "\nAll patches applied successfully!\n";

    }
    catch (const std::exception& e) {
        std::cerr << "\nError: " << e.what() << "\n";
        return 1;
    }

    return 0;
}