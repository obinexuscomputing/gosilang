#!/bin/bash

# Root directory
mkdir -p gosilang

# Create main directory structure
declare -a dirs=(
    # src directory structure
    "src/core/ast/minimizer"
    "src/core/ast/validator"
    "src/core/tokenizer/lexer"
    "src/core/tokenizer/symbols"
    "src/core/minimizer"
    "src/core/parser"
    
    "src/compiler/analyzer/race"
    "src/compiler/analyzer/safety"
    "src/compiler/generator/parallel"
    "src/compiler/generator/generic"
    "src/compiler/optimizer/state"
    "src/compiler/optimizer/parallel"
    
    "src/runtime/concurrency/futures"
    "src/runtime/concurrency/lazy"
    "src/runtime/domasm"
    "src/runtime/web"
    "src/runtime/debug/ploffer"
    "src/runtime/debug/profiler"
    
    # programs directory structure
    "programs/templates"
    "programs/examples"
    "programs/subsystems/sdl2"
    "programs/subsystems/gui"
    "programs/subsystems/test"
    "programs/tests"
    
    # tools directory structure
    "tools/analyzer/race"
    "tools/analyzer/security"
    "tools/visualizer"
    "tools/validator"
    "tools/boost/registry"
    "tools/boost/local"
    
    # docs directory structure
    "docs/specs/language"
    "docs/specs/subsystems"
    "docs/guidelines"
    "docs/examples"
    "docs/books"
    
    # tests directory structure
    "tests/unit"
    "tests/integration"
    "tests/programs"
    "tests/security"
    
    # scripts directory structure
    "scripts/build"
    "scripts/ci"
    "scripts/tools"
)

# Create README files for each directory
create_readme() {
    local dir=$1
    local readme="${dir}/README.md"
    local dirname=$(basename "$dir")
    
    echo "# ${dirname}" > "$readme"
    echo "" >> "$readme"
    echo "## Purpose" >> "$readme"
    echo "Description of the ${dirname} directory and its contents." >> "$readme"
    echo "" >> "$readme"
    echo "## Contents" >> "$readme"
    echo "List of key components and their purposes:" >> "$readme"
    echo "- Component 1: Description" >> "$readme"
    echo "- Component 2: Description" >> "$readme"
}

# Create .gitkeep files to maintain empty directories
create_gitkeep() {
    local dir=$1
    touch "${dir}/.gitkeep"
}

# Main execution
echo "Creating directory structure for Gosilang..."

for dir in "${dirs[@]}"; do
    full_path="gosilang/${dir}"
    echo "Creating: ${full_path}"
    mkdir -p "${full_path}"
    create_readme "${full_path}"
    create_gitkeep "${full_path}"
done
