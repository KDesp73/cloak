#compdef cloak

_cloak_cmd_0 () {
    _path_files
}

_cloak () {
    local -a literals=("decrypt" "rsa" "verify" "-i" "files" "--gitignore-ignore" "init" "ls" "ini" "encrypt" "hash" "--type" "--cloakignore-ignore" "--output" "-o" "generate" "autocomplete" "sign" "--help" "-v" "--version" "signatures" "--key" "--input")

    local -A descriptions
    descriptions[1]="Decrypt a file or a directory"
    descriptions[4]="Specify the input path"
    descriptions[6]="Do not include the gitignore patterns"
    descriptions[7]="Initialize cloak in your project"
    descriptions[8]="Lists the files considered in the encryption process"
    descriptions[10]="Encrypt a file or a directory"
    descriptions[11]="Prints the hash of a file"
    descriptions[12]="Specify the type"
    descriptions[13]="Do not include the cloakignore patterns"
    descriptions[15]="Specify the output path"
    descriptions[16]="Generates various files"
    descriptions[19]="Prints the help message"
    descriptions[20]="Prints the project version"
    descriptions[23]="Specify the encryption key path"

    local -A literal_transitions
    literal_transitions[1]="([1]=7 [3]=2 [18]=2 [20]=2 [7]=2 [8]=3 [19]=2 [10]=4 [11]=5 [13]=2 [16]=6 [21]=2)"
    literal_transitions[3]="([6]=21 [4]=16 [13]=20 [12]=14 [24]=16)"
    literal_transitions[4]="([4]=13 [24]=13)"
    literal_transitions[5]="([4]=10 [24]=10)"
    literal_transitions[6]="([12]=15)"
    literal_transitions[7]="([4]=18 [24]=18)"
    literal_transitions[8]="([14]=9 [15]=9 [23]=10)"
    literal_transitions[11]="([14]=12 [6]=2 [15]=12)"
    literal_transitions[14]="([5]=2 [22]=2)"
    literal_transitions[15]="([2]=2 [9]=2 [17]=2)"
    literal_transitions[17]="([12]=14 [6]=21 [13]=20)"
    literal_transitions[19]="([23]=10)"
    literal_transitions[20]="([12]=14)"
    literal_transitions[21]="([12]=14 [13]=20)"
    literal_transitions[22]="([6]=2)"

    local -A match_anything_transitions
    match_anything_transitions=([16]=17 [10]=2 [18]=8 [9]=19 [13]=11 [12]=22)

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
    declare -A literal_transitions_level_0=([1]="1 3 18 20 7 8 19 10 11 13 16" [20]="12" [11]="6 15" [7]="4" [6]="12" [21]="12 13" [4]="4" [3]="6 4 13 12" [15]="2 9 17" [14]="5 22" [8]="15 23" [22]="6" [17]="12 6 13" [5]="4" [19]="23")
    declare -A literal_transitions_level_1=([7]="24" [5]="24" [4]="24" [3]="24" [1]="21" [8]="14" [11]="14")
    declare -A subword_transitions_level_0=()
    declare -A subword_transitions_level_1=()
    declare -A commands_level_0=()
    declare -A commands_level_1=()
    declare -A specialized_commands_level_0=([16]="0" [18]="0" [10]="0" [13]="0" [12]="0" [9]="0")
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
