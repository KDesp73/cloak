function _cloak_spec_6
set 1 $argv[1]
__fish_complete_path "$1"
end

function _cloak
    set COMP_LINE (commandline --cut-at-cursor)

    set COMP_WORDS
    echo $COMP_LINE | read --tokenize --array COMP_WORDS
    if string match --quiet --regex '.*\s$' $COMP_LINE
        set COMP_CWORD (math (count $COMP_WORDS) + 1)
    else
        set COMP_CWORD (count $COMP_WORDS)
    end

    set literals "decrypt" "rsa" "verify" "-i" "files" "--gitignore-ignore" "init" "ls" "ini" "encrypt" "hash" "--type" "--cloakignore-ignore" "--output" "-o" "generate" "autocomplete" "sign" "--help" "-v" "--version" "--key" "--input"

    set descriptions
    set descriptions[1] "Decrypt a file or a directory"
    set descriptions[4] "Specify the input path"
    set descriptions[6] "Do not include the gitignore patterns"
    set descriptions[7] "Initialize cloak in your project"
    set descriptions[8] "Lists the files considered in the encryption process"
    set descriptions[10] "Encrypt a file or a directory"
    set descriptions[11] "Prints the hash of a file"
    set descriptions[12] "Specify the type"
    set descriptions[13] "Do not include the cloakignore patterns"
    set descriptions[15] "Specify the output path"
    set descriptions[16] "Generates various files"
    set descriptions[19] "Prints the help message"
    set descriptions[20] "Prints the project version"
    set descriptions[22] "Specify the encryption key path"

    set literal_transitions
    set literal_transitions[1] "set inputs 1 3 18 20 7 8 19 10 11 13 16 21; set tos 7 2 2 2 2 3 2 4 5 2 6 2"
    set literal_transitions[3] "set inputs 6 4 13 12 23; set tos 16 20 14 15 20"
    set literal_transitions[4] "set inputs 4 23; set tos 12 12"
    set literal_transitions[5] "set inputs 4 23; set tos 21 21"
    set literal_transitions[6] "set inputs 12; set tos 13"
    set literal_transitions[7] "set inputs 4 23; set tos 8 8"
    set literal_transitions[9] "set inputs 14 15 22; set tos 17 17 21"
    set literal_transitions[10] "set inputs 14 6 15; set tos 11 2 11"
    set literal_transitions[13] "set inputs 2 9 17; set tos 2 2 2"
    set literal_transitions[14] "set inputs 12; set tos 15"
    set literal_transitions[15] "set inputs 5; set tos 2"
    set literal_transitions[16] "set inputs 12 13; set tos 15 14"
    set literal_transitions[18] "set inputs 22; set tos 21"
    set literal_transitions[19] "set inputs 12 6 13; set tos 15 16 14"
    set literal_transitions[22] "set inputs 6; set tos 2"

    set match_anything_transitions_from 20 11 21 8 17 12
    set match_anything_transitions_to 19 22 2 9 18 10

    set state 1
    set word_index 2
    while test $word_index -lt $COMP_CWORD
        set -- word $COMP_WORDS[$word_index]

        if set --query literal_transitions[$state] && test -n $literal_transitions[$state]
            set --erase inputs
            set --erase tos
            eval $literal_transitions[$state]

            if contains -- $word $literals
                set literal_matched 0
                for literal_id in (seq 1 (count $literals))
                    if test $literals[$literal_id] = $word
                        set index (contains --index -- $literal_id $inputs)
                        set state $tos[$index]
                        set word_index (math $word_index + 1)
                        set literal_matched 1
                        break
                    end
                end
                if test $literal_matched -ne 0
                    continue
                end
            end
        end

        if set --query match_anything_transitions_from[$state] && test -n $match_anything_transitions_from[$state]
            set index (contains --index -- $state $match_anything_transitions_from)
            set state $match_anything_transitions_to[$index]
            set word_index (math $word_index + 1)
            continue
        end

        return 1
    end

    if set --query literal_transitions[$state] && test -n $literal_transitions[$state]
        set --erase inputs
        set --erase tos
        eval $literal_transitions[$state]
        for literal_id in $inputs
            if test -n $descriptions[$literal_id]
                printf '%s\t%s\n' $literals[$literal_id] $descriptions[$literal_id]
            else
                printf '%s\n' $literals[$literal_id]
            end
        end
    end

    set specialized_command_states 21 17 12 20 8 11
    set specialized_command_ids 6 6 6 6 6 6
    if contains $state $specialized_command_states
        set index (contains --index $state $specialized_command_states)
        set function_id $specialized_command_ids[$index]
        set function_name _cloak_spec_$function_id
        set --erase inputs
        set --erase tos
        set lines (eval $function_name $COMP_WORDS[$COMP_CWORD])
        for line in $lines
            printf '%s\n' $line
        end
    end

    return 0
end

complete --command cloak --no-files --arguments "(_cloak)"
