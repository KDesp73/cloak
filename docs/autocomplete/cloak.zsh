#compdef cloak

_cloak_cmd_0 () {
    _path_files
}

_cloak () {
    local -a literals=("decrypt" "rsa" "-i" "--gitignore-ignore" "init" "ls" "ini" "encrypt" "hash" "--type" "--cloakignore-ignore" "--output" "-o" "generate" "autocomplete" "--help" "-v" "--version" "signatures" "--key" "--input")

    local -A descriptions
    descriptions[1]="Decrypt a file or a directory"
    descriptions[3]="Specify the input path"
    descriptions[4]="Do not include the gitignore patterns"
    descriptions[5]="Initialize cloak in your project"
    descriptions[6]="Lists the files considered in the encryption process"
    descriptions[8]="Encrypt a file or a directory"
    descriptions[9]="Prints the hash of a file"
    descriptions[10]="Specify the type"
    descriptions[11]="Do not include the cloakignore patterns"
    descriptions[13]="Specify the output path"
    descriptions[14]="Generates various files"
    descriptions[16]="Prints the help message"
    descriptions[17]="Prints the project version"
    descriptions[20]="Specify the encryption key path"

    local -A literal_transitions
    literal_transitions[1]="([1]=7 [17]=2 [5]=2 [6]=3 [16]=2 [8]=4 [9]=5 [11]=2 [14]=6 [18]=2)"
    literal_transitions[3]="([4]=13 [3]=22 [11]=15 [10]=14 [21]=22)"
    literal_transitions[4]="([3]=12 [21]=12)"
    literal_transitions[5]="([3]=10 [21]=10)"
    literal_transitions[6]="([10]=11)"
    literal_transitions[7]="([3]=18 [21]=18)"
    literal_transitions[8]="([12]=9 [13]=9 [20]=10)"
    literal_transitions[11]="([2]=2 [7]=2 [15]=2)"
    literal_transitions[13]="([10]=14 [11]=15)"
    literal_transitions[14]="([19]=2)"
    literal_transitions[15]="([10]=14)"
    literal_transitions[16]="([12]=17 [4]=2 [13]=17)"
    literal_transitions[19]="([20]=10)"
    literal_transitions[20]="([4]=2)"
    literal_transitions[21]="([10]=14 [4]=13 [11]=15)"

    local -A match_anything_transitions
    match_anything_transitions=([10]=2 [18]=8 [9]=19 [12]=16 [17]=20 [22]=21)

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
    declare -A literal_transitions_level_0=([1]="1 17 5 6 16 8 9 11 14" [20]="4" [7]="3" [16]="4 13" [6]="10" [11]="2 7 15" [4]="3" [21]="10 4 11" [3]="4 3 11 10" [15]="10" [13]="10 11" [8]="13 20" [14]="19" [19]="20" [5]="3")
    declare -A literal_transitions_level_1=([16]="12" [5]="21" [4]="21" [3]="21" [1]="18" [8]="12" [7]="21")
    declare -A subword_transitions_level_0=()
    declare -A subword_transitions_level_1=()
    declare -A commands_level_0=()
    declare -A commands_level_1=()
    declare -A specialized_commands_level_0=([18]="0" [17]="0" [10]="0" [22]="0" [12]="0" [9]="0")
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
