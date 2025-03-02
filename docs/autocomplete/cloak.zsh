#compdef cloak

_cloak_cmd_0 () {
    _path_files
}

_cloak () {
    local -a literals=("decrypt" "-i" "--gitignore-ignore" "init" "ls" "ini" "generate" "hash" "encrypt" "--cloakignore-ignore" "--output" "-o" "--type" "autocomplete" "--help" "-v" "--version" "--key" "--input")

    local -A descriptions
    descriptions[1]="Decrypt a file or a directory"
    descriptions[2]="Specify the input path"
    descriptions[3]="Do not include the gitignore patterns"
    descriptions[4]="Initialize cloak in your project"
    descriptions[5]="Lists the files considered in the encryption process"
    descriptions[7]="Generates various files"
    descriptions[8]="Prints the hash of a file"
    descriptions[9]="Encrypt a file or a directory"
    descriptions[10]="Do not include the cloakignore patterns"
    descriptions[12]="Specify the output path"
    descriptions[13]="Specify the type"
    descriptions[15]="Prints the help message"
    descriptions[16]="Prints the project version"
    descriptions[18]="Specify the encryption key path"

    local -A literal_transitions
    literal_transitions[1]="([1]=7 [16]=2 [4]=2 [5]=3 [15]=2 [7]=4 [8]=5 [9]=6 [10]=2 [17]=2)"
    literal_transitions[3]="([2]=13 [10]=2 [3]=15 [19]=13)"
    literal_transitions[4]="([13]=16)"
    literal_transitions[5]="([2]=12 [19]=12)"
    literal_transitions[6]="([2]=17 [19]=17)"
    literal_transitions[7]="([2]=10 [19]=10)"
    literal_transitions[9]="([18]=12)"
    literal_transitions[11]="([11]=8 [12]=8 [18]=12)"
    literal_transitions[14]="([3]=15 [10]=2)"
    literal_transitions[15]="([10]=2)"
    literal_transitions[16]="([6]=2 [14]=2)"
    literal_transitions[18]="([11]=19 [3]=2 [12]=19)"
    literal_transitions[20]="([3]=2)"

    local -A match_anything_transitions
    match_anything_transitions=([10]=11 [19]=20 [13]=14 [8]=9 [17]=18 [12]=2)

    declare -A subword_transitions

    local state=1
    local word_index=2
    while [[ $word_index -lt $CURRENT ]]; do
        if [[ -v "literal_transitions[$state]" ]]; then
            local -A state_transitions
            eval "state_transitions=${literal_transitions[$state]}"

            local word=${words[$word_index]}
            local word_matched=0
            for ((literal_id = 1; literal_id <= $#literals; literal_id++)); do
                if [[ ${literals[$literal_id]} = "$word" ]]; then
                    if [[ -v "state_transitions[$literal_id]" ]]; then
                        state=${state_transitions[$literal_id]}
                        word_index=$((word_index + 1))
                        word_matched=1
                        break
                    fi
                fi
            done
            if [[ $word_matched -ne 0 ]]; then
                continue
            fi
        fi

        if [[ -v "match_anything_transitions[$state]" ]]; then
            state=${match_anything_transitions[$state]}
            word_index=$((word_index + 1))
            continue
        fi

        return 1
    done
    declare -A literal_transitions_level_0=([3]="2 10 3" [14]="3 10" [1]="1 16 4 5 15 7 8 9 10" [20]="3" [7]="2" [16]="6 14" [6]="2" [18]="3 12" [4]="13" [11]="12 18" [15]="10" [5]="2" [9]="18")
    declare -A literal_transitions_level_1=([18]="11" [6]="19" [11]="11" [5]="19" [3]="19" [1]="17" [7]="19")
    declare -A subword_transitions_level_0=()
    declare -A subword_transitions_level_1=()
    declare -A commands_level_0=()
    declare -A commands_level_1=()
    declare -A specialized_commands_level_0=([17]="0" [10]="0" [19]="0" [13]="0" [8]="0" [12]="0")
    declare -A specialized_commands_level_1=()

     local max_fallback_level=1
     for (( fallback_level=0; fallback_level <= max_fallback_level; fallback_level++ )) {
         completions_no_description_trailing_space=()
         completions_no_description_no_trailing_space=()
         completions_trailing_space=()
         suffixes_trailing_space=()
         descriptions_trailing_space=()
         completions_no_trailing_space=()
         suffixes_no_trailing_space=()
         descriptions_no_trailing_space=()
         matches=()

         declare literal_transitions_name=literal_transitions_level_${fallback_level}
         eval "declare initializer=\${${literal_transitions_name}[$state]}"
         eval "declare -a transitions=($initializer)"
         for literal_id in "${transitions[@]}"; do
             if [[ -v "descriptions[$literal_id]" ]]; then
                 completions_trailing_space+=("${literals[$literal_id]}")
                 suffixes_trailing_space+=("${literals[$literal_id]}")
                 descriptions_trailing_space+=("${descriptions[$literal_id]}")
             else
                 completions_no_description_trailing_space+=("${literals[$literal_id]}")
             fi
         done

         declare subword_transitions_name=subword_transitions_level_${fallback_level}
         eval "declare initializer=\${${subword_transitions_name}[$state]}"
         eval "declare -a transitions=($initializer)"
         for subword_id in "${transitions[@]}"; do
             _cloak_subword_${subword_id} complete "${words[$CURRENT]}"
             completions_no_description_trailing_space+=("${subword_completions_no_description_trailing_space[@]}")
             completions_trailing_space+=("${subword_completions_trailing_space[@]}")
             completions_no_trailing_space+=("${subword_completions_no_trailing_space[@]}")
             suffixes_no_trailing_space+=("${subword_suffixes_no_trailing_space[@]}")
             suffixes_trailing_space+=("${subword_suffixes_trailing_space[@]}")
             descriptions_trailing_space+=("${subword_descriptions_trailing_space[@]}")
             descriptions_no_trailing_space+=("${subword_descriptions_no_trailing_space[@]}")
         done

         declare commands_name=commands_level_${fallback_level}
         eval "declare initializer=\${${commands_name}[$state]}"
         eval "declare -a transitions=($initializer)"
         for command_id in "${transitions[@]}"; do
             local output=$(_cloak_cmd_${command_id} "${words[$CURRENT]}")
             local -a command_completions=("${(@f)output}")
             for line in ${command_completions[@]}; do
                 local parts=(${(@s:	:)line})
                 if [[ -v "parts[2]" ]]; then
                     completions_trailing_space+=("${parts[1]}")
                     suffixes_trailing_space+=("${parts[1]}")
                     descriptions_trailing_space+=("${parts[2]}")
                 else
                     completions_no_description_trailing_space+=("${parts[1]}")
                 fi
             done
         done

         declare specialized_commands_name=specialized_commands_level_${fallback_level}
         eval "declare initializer=\${${specialized_commands_name}[$state]}"
         eval "declare -a transitions=($initializer)"
         for command_id in "${transitions[@]}"; do
             _cloak_cmd_${command_id} ${words[$CURRENT]}
         done

         local maxlen=0
         for suffix in ${suffixes_trailing_space[@]}; do
             if [[ ${#suffix} -gt $maxlen ]]; then
                 maxlen=${#suffix}
             fi
         done
         for suffix in ${suffixes_no_trailing_space[@]}; do
             if [[ ${#suffix} -gt $maxlen ]]; then
                 maxlen=${#suffix}
             fi
         done

         for ((i = 1; i <= $#suffixes_trailing_space; i++)); do
             if [[ -z ${descriptions_trailing_space[$i]} ]]; then
                 descriptions_trailing_space[$i]="${(r($maxlen)( ))${suffixes_trailing_space[$i]}}"
             else
                 descriptions_trailing_space[$i]="${(r($maxlen)( ))${suffixes_trailing_space[$i]}} -- ${descriptions_trailing_space[$i]}"
             fi
         done

         for ((i = 1; i <= $#suffixes_no_trailing_space; i++)); do
             if [[ -z ${descriptions_no_trailing_space[$i]} ]]; then
                 descriptions_no_trailing_space[$i]="${(r($maxlen)( ))${suffixes_no_trailing_space[$i]}}"
             else
                 descriptions_no_trailing_space[$i]="${(r($maxlen)( ))${suffixes_no_trailing_space[$i]}} -- ${descriptions_no_trailing_space[$i]}"
             fi
         done

         compadd -O m -a completions_no_description_trailing_space; matches+=("${m[@]}")
         compadd -O m -a completions_no_description_no_trailing_space; matches+=("${m[@]}")
         compadd -O m -a completions_trailing_space; matches+=("${m[@]}")
         compadd -O m -a completions_no_trailing_space; matches+=("${m[@]}")

         if [[ ${#matches} -gt 0 ]]; then
             compadd -Q -a completions_no_description_trailing_space
             compadd -Q -S ' ' -a completions_no_description_no_trailing_space
             compadd -l -Q -a -d descriptions_trailing_space completions_trailing_space
             compadd -l -Q -S '' -a -d descriptions_no_trailing_space completions_no_trailing_space
             return 0
         fi
     }
}

if [[ $ZSH_EVAL_CONTEXT =~ :file$ ]]; then
    compdef _cloak cloak
else
    _cloak
fi
